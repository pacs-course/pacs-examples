
#ifndef HAVE_MIDPOINT_H
#define HAVE_MIDPOINT_H
#include <functional>

extern "C"
{
  double
  integrate (std::function<double (double)>, double, double);
}

#endif
