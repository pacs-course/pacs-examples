
#ifndef HAVE_TRAPEZOIDAL_H
#define HAVE_TRAPEZOIDAL_H
#include <functional>

extern "C"
{
  double
  integrate (std::function<double (double)>, double, double);
}

#endif
