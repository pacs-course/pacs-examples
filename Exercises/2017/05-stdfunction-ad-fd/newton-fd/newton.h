#ifndef HAVE_NEWTON_H
#  define HAVE_NEWTON_H

#include <iostream>
#include <limits>
#include <cmath>
#include <functional>

class
newton_solver
{
 private:

  std::function<double (const double)> fun;

  int    maxit;
  double funtol;
  double xtol;
  double delta;
  double dx;
  double x;
  double dfdx;
  double r;
  int    iter;
  
 public:
  
  newton_solver (std::function<double (const double)> fun_,
                 const int maxit_ = 100,
                 const double funtol_ = std::numeric_limits<double>::epsilon () * 1000.0,
                 const double xtol_ = std::numeric_limits<double>::epsilon () * 1000.0,
                 const double delta_ = 10.0 * std::numeric_limits<double>::epsilon ())
    : fun (fun_), maxit (maxit_), funtol (funtol_), xtol (xtol_), delta (delta_)
  { x = 0; dfdx = 0; r = 0; iter = 0; }

  double
  prime (double y)
  { return (fun (y + delta/2) - fun (y - delta/2)) / (delta); }

  void
  solve (const double x0)
  {
    x = x0;
    for (iter = 0; iter < maxit; ++iter)
      {
        r    = fun (x);
        if (std::abs (r) < funtol) break;
        dfdx = prime (x);
        dx   = - r / dfdx;
        if (std::abs (dx) < xtol) break;
        x += dx;
      }
  }

  double
  get_result ()
  { return x; };

  double
  get_residual ()
  { return r; };

  int
  get_iter ()
  { return iter; };


};

#endif 
