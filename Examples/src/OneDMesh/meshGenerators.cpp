#include "meshGenerators.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
// #include "rk45.hpp"
//  use the new version
#include "RKF.hpp"
namespace Geometry
{
MeshNodes
Uniform::operator()() const
{
  auto const &n = this->M_num_elements;
  auto const &a = this->M_domain.left();
  auto const &b = this->M_domain.right();
  if(n == 0)
    throw std::invalid_argument("Uniform mesh requires at least one interval");
  MeshNodes    mesh(n + 1);
  double const h = (b - a) / static_cast<double>(n);
#pragma omp parallel for
  for(auto i = 0u; i < n; ++i)
    mesh[i] = a + h * i;
  mesh[n] = b;
  return mesh;
}

MeshNodes
VariableSize::operator()() const
{
  // calculate dy/dx = h^-1(x)
  MeshNodes  mesh;
  auto const t0 = this->M_domain.left();
  auto const T = this->M_domain.right();
  double const y0 = 0.0;
  auto const h_max = (T - t0) / 4.;
  auto const h_initial = h_max / 100.;
  double constexpr final_error = 1e-2;
  std::size_t constexpr maxSteps = 20000;
  if(M_num_elements < 2u)
    throw std::invalid_argument(
      "VariableSize mesh requires max_num_elements >= 2");

  auto fun = [this](double x, double) {
    const double h_value = this->M_h(x);
    if(h_value <= 0.0)
      throw std::domain_error("Spacing function must be strictly positive");
    return 1. / h_value;
  };
  apsc::RKF<apsc::RKFScheme::RK45_t, apsc::RKFKind::SCALAR> solver{fun};
  // NOTE This part is a little cumbersome but needed to update from a
  // previous version of the ODEs solver which returned the results in a
  // different structure. I use a block scope to delete variables that are
  // only used to interface with the new version of the RKF code
  std::vector<std::pair<double, double>> solution;
  {
    auto RKFsolution = solver(t0, T, y0, h_initial, final_error, maxSteps);
    if(RKFsolution.failed)
      std::cerr << "MESH GENERATION HAD A PROBLEM. CONTINUING BUT CHECK!\n";
    if(RKFsolution.time.empty() || RKFsolution.y.empty())
      throw std::runtime_error(
        "VariableSize mesh generation failed: empty RKF solution");
    solution.resize(RKFsolution.time.size());
    /*for loop version */
#pragma omp parallel for shared(solution, RKFsolution) default(none)
    for(auto i = 0u; i < RKFsolution.time.size(); ++i)
      {
        solution[i] = std::make_pair(RKFsolution.time[i], RKFsolution.y[i]);
      }
    /* With a c++ algorithm
          std::transform(std::execution::par,
                        RKFsolution.time.begin(), RKFsolution.time.end(),
                        RKFsolution.y.begin(), solution.begin(),
                        [](double const &x, double const &y) {
                        return std::make_pair(x, y);
                         });
    */
    // Here RKFResult is killed
  }

  auto lastValue = solution.back().second;
  if(lastValue <= 0.0)
    throw std::runtime_error(
      "VariableSize mesh generation failed: non-positive integrated spacing");
  // make it an integer
  std::size_t numElements =
    std::max(static_cast<std::size_t>(std::round(lastValue)),
             static_cast<std::size_t>(2));
  if(numElements > M_num_elements)
    throw std::runtime_error(
      "VariableSize mesh generation failed: required elements exceed maximum");
  // rescale
  using pDouble = std::pair<double, double>;
  double scaling = numElements / lastValue;
// rescale
#pragma omp parallel for shared(solution) firstprivate(scaling) default(none)
  for(auto i = 0u; i < solution.size(); ++i)
    {
      solution[i].second *= scaling;
    }
  // Now I need to interpolate the solution
  mesh.resize(numElements + 1);   // I need to store the nodes
  mesh.front() = M_domain.left(); // first node
  // Now the internal nodes
  auto pos = solution.cbegin() + 1;
  for(std::size_t i = 1u; i < numElements; ++i)
    {
      // find_if finds the first element satisfying the predicate
      // Here I am using upper_bound to find the first element
      // that is greater than i.second because it is more efficient than
      // find_if when the searched range is ordered. Indeed I assume
      // (precondition) that that solution.second is ordered (increasingly).
      // Note the use of the projector to extract the second element
      // std::ranges::less is the default comparator so I could have used {}
      auto found = std::ranges::upper_bound(
        pos, solution.cend(), i, std::ranges::less{}, &pDouble::second);
      // This is the solution with find_if
      // std::find_if(pos, solution.cend(),
      //              [i](pDouble const &value) { return value.second > i; });
      if(found == solution.cend())
        throw std::runtime_error("VariableSize mesh generation failed: "
                                 "cannot locate interpolation node");
      pos = found - 1;
      auto xpos1 = pos->first;
      auto ypos1 = pos->second;
      auto xpos2 = found->first;
      auto ypos2 = found->second;
      // Linear interpolation
      // Division by zero should not happen. But.. just in case
      if(std::abs(ypos2 - ypos1) < std::numeric_limits<double>::epsilon())
        throw std::runtime_error("VariableSize mesh generation failed: "
                                 "degenerate interpolation interval");
      auto xpos = (xpos1 * (ypos2 - i) + xpos2 * (i - ypos1)) / (ypos2 - ypos1);
      mesh[i] = xpos;
    }
  // The last node
  mesh.back() = M_domain.right();
  return mesh;
}

} // namespace Geometry
