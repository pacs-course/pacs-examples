#include "trapezoidal.h"
#include <cmath>
#include <memory>
#include <iostream>

double trapezoidal_quadrature::operator()
  (std::function<double (double)> f, double a, double b)
{ return ((b - a) * (.5 * f(b) + .5 * f(a))); };


quadrature_handle
trapezoidal_quadrature_builder ()
{
  return quadrature_handle (new trapezoidal_quadrature);
};
  
void
register_rules ()
{
  auto &f = quadrature_factory::instance ();
  f.add ("trapezoidal", trapezoidal_quadrature_builder);
};
