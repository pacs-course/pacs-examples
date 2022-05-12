#include "trapezoidal.hpp"

#include <cmath>

double
integrate(std::function<double(double)> f, double a, double b)
{
  return ((b - a) * (0.5 * f(b) + 0.5 * f(a)));
}
