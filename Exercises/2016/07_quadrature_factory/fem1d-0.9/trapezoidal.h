
#ifndef HAVE_TRAPEZOIDAL_H
#define HAVE_TRAPEZOIDAL_H

#include "abstract_quadrature.h"
#include <functional>

class
trapezoidal_quadrature : public abstract_quadrature
{
  double operator()
  (std::function<double (double)> f, double a, double b);
};

extern "C"
{ void register_rules (); };


#endif
