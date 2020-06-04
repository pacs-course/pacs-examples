#ifndef HAVE_TRAPEZOIDAL_HPP
#define HAVE_TRAPEZOIDAL_HPP

#include <functional>

extern "C"
{
  double
  integrate(std::function<double(double)>, double, double);
}

#endif
