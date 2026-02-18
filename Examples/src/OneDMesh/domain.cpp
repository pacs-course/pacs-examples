#include "domain.hpp"
#include <cmath>
namespace Geometry
{
Domain1D::Domain1D(double a, double b) noexcept : M_a(a), M_b(b) {}
double
Domain1D::length() const noexcept
{
  return std::abs(M_b - M_a);
}
} // namespace Geometry
