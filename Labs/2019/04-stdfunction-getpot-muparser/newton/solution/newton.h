#include <iostream>
#include <limits>
#include <cmath>
#include <functional>

class
newton_solver
{
 private:

  std::function<double (const double)> fun;
  std::function<double (const double)> prime;
  int    maxit;
  double funtol;
  double xtol;
  double x;
  double dfdx;
  double dx;
  double r;
  int    iter;
  
 public:
  
  newton_solver (std::function<double (const double)> fun_,
                 std::function<double (const double)> prime_,
                 const int maxit_ = 100,
                 const double funtol_ = std::numeric_limits<double>::epsilon () * 1000.0,
                 const double xtol_ = std::numeric_limits<double>::epsilon () * 1000.0)
    : fun (fun_), prime (prime_), maxit (maxit_), funtol (funtol_), xtol (xtol_)
  { x = 0; dfdx = 0; r = 0; iter = 0; }
    

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
