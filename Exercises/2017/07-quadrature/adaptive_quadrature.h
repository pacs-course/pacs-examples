#ifndef HAVE_ADAPTIVE_QUADRATURE_H
#define HAVE_ADAPTIVE_QUADRATURE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <functional>

class
adaptive_quadrature
{
private :

  int depth = 0;
  int depth_reached = 0;

  std::function<double (double)> f;
  const double start, finish;
  const double tol;
  const int maxdepth;

  double
  refine (double a, double b, double oldval);
  
  double
  trapz (std::function<double (double)> f, double a, double b);

public :

  adaptive_quadrature (std::function<double (double)> f_, double start_,
                       double finish_, double tol_ = 1.0e-12, int maxdepth_ = 40)
    : f(f_), start (start_), finish (finish_), tol (tol_), maxdepth (maxdepth_)
  {};
  
  double
  integrate ();

};

#endif
