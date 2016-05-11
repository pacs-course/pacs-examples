#include "midpoint.h"
#include <cmath>
#include <iostream>
#include <iomanip>

double
integrate (std::function<double (double)> f, double a, double b)
{
  return ((b - a) * f(.5*b + .5*a));
};


