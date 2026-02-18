#include "mesh.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>
namespace Geometry
{
Mesh1D::Mesh1D(Domain1D const &d, std::size_t n) : myDomain(d)
{
  Uniform g(d, n);
  myNodes = g();
};

double
Mesh1D::hmax() const
{
  if(myNodes.size() < 2u)
    throw std::logic_error("hmax() requires at least two mesh nodes");
  double max_h = 0.0;
  for(std::size_t i = 1u; i < myNodes.size(); ++i)
    {
      max_h = std::max(max_h, myNodes[i] - myNodes[i - 1u]);
    }
  if(max_h <= 0.0)
    throw std::logic_error("Mesh nodes must be strictly increasing");
  return max_h;
}

double
Mesh1D::hmin() const
{
  if(myNodes.size() < 2u)
    throw std::logic_error("hmin() requires at least two mesh nodes");
  double min_h = std::numeric_limits<double>::max();
  for(std::size_t i = 1u; i < myNodes.size(); ++i)
    min_h = std::min(min_h, myNodes[i] - myNodes[i - 1u]);
  return min_h;
}

void
Mesh1D::reset(OneDMeshGenerator const &mg)
{
  myDomain = mg.getDomain();
  myNodes = mg();
}

} // namespace Geometry
