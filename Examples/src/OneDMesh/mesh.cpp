#include "mesh.hpp"
#include <algorithm>
#include <numeric>
namespace Geometry
{
Mesh1D::Mesh1D(Domain1D const &d, unsigned int const &n) : myDomain(d)
{
  Uniform g(d, n);
  myNodes = g();
};

double
Mesh1D::hmax() const
{
  std::vector<double> tmp(myNodes.size());
  std::adjacent_difference(myNodes.begin(), myNodes.end(), tmp.begin());
  return *std::max_element(++tmp.begin(), tmp.end());
}

double
Mesh1D::hmin() const
{
  std::vector<double> tmp(myNodes.size());
  std::adjacent_difference(myNodes.begin(), myNodes.end(), tmp.begin());
  return *std::min_element(++tmp.begin(), tmp.end());
}

void
Mesh1D::reset(OneDMeshGenerator const &mg)
{
  myDomain = mg.getDomain();
  myNodes = mg();
}

} // namespace Geometry
