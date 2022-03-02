#ifndef NEWTON_H
#define NEWTON_H

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>

class newton_solver
{
public:
  newton_solver(const std::function<double(const double &)> &fun_,
                const std::function<double(const double &)> &prime_,
                const int                                    n_max_it_ = 100,
                const double tol_fun_ = std::numeric_limits<double>::epsilon() *
                                        1000.0,
                const double tol_x_ = std::numeric_limits<double>::epsilon() *
                                      1000.0)
    : fun(fun_)
    , prime(prime_)
    , n_max_it(n_max_it_)
    , tol_fun(tol_fun_)
    , tol_x(tol_x_)
  {
    x    = 0;
    dfdx = 0;
    res  = 0;
    iter = 0;
  }

  void
  solve(const double &x0)
  {
    x = x0;
    for (iter = 0; iter < n_max_it; ++iter)
      {
        res = fun(x);
        if (std::abs(res) < tol_fun)
          break;
        dfdx = prime(x);
        dx   = -res / dfdx;
        if (std::abs(dx) < tol_x)
          break;
        x += dx;
      }
  }

  double
  get_result()
  {
    return x;
  };

  double
  get_residual()
  {
    return res;
  };

  int
  get_iter()
  {
    return iter;
  };

private:
  std::function<double(const double &)> fun;
  std::function<double(const double &)> prime;

  const int    n_max_it;
  const double tol_fun;
  const double tol_x;

  double x;
  double dfdx;
  double dx;
  double res;
  int    iter;
};

#endif /* NEWTON_H */
