#ifndef NEWTON_H
#define NEWTON_H

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>

class NewtonSolver
{
public:
  NewtonSolver(const std::function<double(const double &)> &fun_,
                const std::function<double(const double &)> &dfun_,
                const unsigned int                           n_max_it_ = 100,
                const double tol_fun_ = std::numeric_limits<double>::epsilon() *
                                        1000.0,
                const double tol_x_ = std::numeric_limits<double>::epsilon() *
                                      1000.0)
    : m_fun(fun_)
    , m_dfun(dfun_)
    , m_n_max_it(n_max_it_)
    , m_tol_fun(tol_fun_)
    , m_tol_x(tol_x_)
    , m_x(0)
    , m_df_dx(0)
    , m_dx(0)
    , m_res(0)
    , m_iter(0)
  {}

  void
  solve(const double &x0)
  {
    m_x = x0;
    for (m_iter = 0; m_iter < m_n_max_it; ++m_iter)
      {
        m_res = m_fun(m_x);

        if (std::abs(m_res) < m_tol_fun)
          break;

        m_df_dx = m_dfun(m_x);

        m_dx = -m_res / m_df_dx;
        m_x += m_dx;

        if (std::abs(m_dx) < m_tol_x)
          break;
      }
  }

  double
  get_result() const
  {
    return m_x;
  };

  double
  get_residual() const
  {
    return m_res;
  };

  unsigned int
  get_iter() const
  {
    return m_iter;
  };

private:
  std::function<double(const double &)> m_fun;
  std::function<double(const double &)> m_dfun;

  const unsigned int m_n_max_it;
  const double       m_tol_fun;
  const double       m_tol_x;

  double       m_x;
  double       m_df_dx;
  double       m_dx;
  double       m_res;
  unsigned int m_iter;
};

#endif /* NEWTON_H */
