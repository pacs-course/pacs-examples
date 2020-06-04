#include "trapezoidal.hpp"

#include <cmath>

double
integrate(std::function<double(double)> f, double a, double b)
{
  return ((b - a) * (.5 * f(b) + .5 * f(a)));
}
