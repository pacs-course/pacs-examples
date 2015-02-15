#include "domain.hpp"
#include <cmath>
namespace Geometry
{
  Domain1D::Domain1D(double const & a, double const & b):
    M_a(a),M_b(b)
  {
  }
  double Domain1D::length()const
  {
    return std::abs(M_b-M_a);
  }
}// end namespace
