
#ifndef HAVE_ADAPTIVE_QUADRATURE_H
#define HAVE_ADAPTIVE_QUADRATURE_H
#include <functional>

extern "C"
{
  double
  integrate (std::function<double (double)>, double, double);
}

#endif
