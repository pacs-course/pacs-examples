#include "adaptive_quadrature.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>

#define tol 1e-12
#define maxdepth 40

int depth         = 0;
int depth_reached = 0;

double
trapz(std::function<double(double)> f, double a, double b)
{
  double retval = 0.0;
  double h      = b - a;
  retval        = .5 * h * (f(a) + f(b));
  return retval;
}

double
refine(std::function<double(double)> f, double a, double b, double oldval)
{
  ++depth;
  if (depth > depth_reached)
    depth_reached = depth;

  double c = .5 * a + .5 * b;

  double oldval_l = trapz(f, a, c);
  double oldval_r = trapz(f, c, b);
  double newval   = oldval_l + oldval_r;

  double retval = 0.0;

  if (depth >= maxdepth || fabs(newval - oldval) <= tol)
    retval = newval;
  else
    {
      retval += refine(f, a, c, oldval_l);
      retval += refine(f, c, b, oldval_r);
    }

  --depth;

  return retval;
}


double
integrate(std::function<double(double)> f, double a, double b)
{
  double res    = 0;
  depth         = 0;
  depth_reached = 0;
  res           = refine(f, a, b, 0.0);

  return res;
}
