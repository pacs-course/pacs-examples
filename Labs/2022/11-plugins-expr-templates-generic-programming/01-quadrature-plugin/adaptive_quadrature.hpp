#ifndef HAVE_ADAPTIVE_QUADRATURE_HPP
#define HAVE_ADAPTIVE_QUADRATURE_HPP

#include <functional>

extern "C"
{
  double
  integrate(std::function<double(double)>, double, double);
}

#endif
