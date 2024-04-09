#include "numerical_integration.hpp"
#ifdef PALALLELCPP
#include <execution>
#endif
namespace apsc::NumericalIntegration
{
// I need to copy the QuadratureRule not its handler!
/*Quadrature & Quadrature::operator=(Quadrature const & rhs){
  if(this!=&rhs){
    mesh_=rhs.mesh_;
    // Resets the current value replacing with the pointer
    // returned by rhs._rule
    rule_=rhs.rule_->clone();
  }
  return *this;
}
*/
/*
I show different implementations:
- Standard for loop
- OpenMP
- Parallel STL
*/
double
Quadrature::apply(FunPoint const &f) const
{
  double result(0);
#ifndef PARALLELCPP
#ifdef _OPENMP
#pragma omp parallel for reduction(+ : result) shared(f)
#endif
  for(unsigned int i = 0u; i < mesh_.numNodes() - 1; ++i)
    {
      double const a = mesh_[i];
      double const b = mesh_[i + 1];
      result += rule_->apply(f, a, b);
    }
#else
  result = std::transform_reduce(std::execution::par_unseq, mesh_.begin(),
                                 mesh_.end() - 1, 0.0, std::plus<double>(),
                                 [this, &f](double const a, double const b) {
                                   return this->rule_->apply(f, a, b);
                                 });
#endif
  return result;
}

} // namespace apsc::NumericalIntegration
