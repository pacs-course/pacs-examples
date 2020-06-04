#ifndef HAVE_MIDPOINT_HPP
#define HAVE_MIDPOINT_HPP

#include <functional>

extern "C"
{
  double
  integrate(std::function<double(double)>, double, double);
}

#endif
