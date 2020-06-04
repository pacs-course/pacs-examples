#include "midpoint.hpp"

#include <cmath>

double
integrate(std::function<double(double)> f, double a, double b)
{
  return ((b - a) * f(.5 * b + .5 * a));
}
