#include "midpoint.hpp"

#include <cmath>

double
integrate(std::function<double(double)> f, double a, double b)
{
  return ((b - a) * f(0.5 * b + 0.5 * a));
}
