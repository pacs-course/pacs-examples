#ifndef HH_QMR___HH
#define HH_QMR___HH
//*****************************************************************
// Iterative template routine -- QMR
//
// QMR.h solves the unsymmetric linear system Ax = b using the
// Quasi-Minimal Residual method following the algorithm as described
// on p. 24 in the SIAM Templates book.
//
//   -------------------------------------------------------------
//   return value     indicates
//   ------------     ---------------------
//        0           convergence within max_iter iterations
//        1           no convergence after max_iter iterations
//                    breakdown in:
//        2             rho
//        3             beta
//        4             gamma
//        5             delta
//        6             ep
//        7             xi
//   -------------------------------------------------------------
//
// Upon successful return, output arguments have the following values:
//
//        x  --  approximate solution to Ax=b
// max_iter  --  the number of iterations performed before the
//               tolerance was reached
//      tol  --  the residual after the final iteration
//
//*****************************************************************


#include "LinearAlgebraTraits.hpp"

#include <cmath>

template <class Matrix,
          class Vector,
          class Preconditioner1,
          class Preconditioner2>
int
QMR(const Matrix &           A,
    Vector &                 x,
    const Vector &           b,
    const Preconditioner1 &  M1,
    const Preconditioner2 &  M2,
    int &                    max_iter,
    typename Vector::Scalar &tol)
{
  using LinearAlgebra::dot;
  using LinearAlgebra::norm;
  using Real = typename Vector::Scalar;

  Real resid;

  Real rho(0.), rho_1(0.), xi(0.), gamma(0.), gamma_1(0.), theta(0.),
    theta_1(0.);
  Real eta(0.), delta(0.), ep(0.), beta(0.);

  Vector r, v_tld, y, w_tld, z;
  Vector v, w, y_tld, z_tld;
  Vector p, q, p_tld, d, s;

  Real normb = norm(b);

  // We need the left preconitioner
  Matrix          At = A.transpose();
  Preconditioner1 M1t(At);
  Preconditioner2 M2t(At);

  r = b - A * x;

  if (normb == 0.0)
    normb = 1;

  if ((resid = norm(r) / normb) <= tol)
    {
      tol      = resid;
      max_iter = 0;
      return 0;
    }

  v_tld = r;
  y     = M1.solve(v_tld);
  rho   = norm(y);

  w_tld = r;
  z     = M2t.solve(w_tld);
  xi    = norm(z);

  gamma = 1.0;
  eta   = -1.0;
  theta = 0.0;

  for (int i = 1; i <= max_iter; i++)
    {
      if (rho == 0.0)
        return 2; // return on breakdown

      if (xi == 0.0)
        return 7; // return on breakdown

      v = (1. / rho) * v_tld;
      y = (1. / rho) * y;

      w = (1. / xi) * w_tld;
      z = (1. / xi) * z;

      delta = dot(z, y);
      if (delta == 0.0)
        return 5; // return on breakdown

      y_tld = M2.solve(y); // apply preconditioners
      z_tld = M1t.solve(z);

      if (i > 1)
        {
          p = y_tld - (xi * delta / ep) * p;
          q = z_tld - (rho * delta / ep) * q;
        }
      else
        {
          p = y_tld;
          q = z_tld;
        }

      p_tld = A * p;
      ep    = dot(q, p_tld);
      if (ep == 0.0)
        return 6; // return on breakdown

      beta = ep / delta;
      if (beta == 0.0)
        return 3; // return on breakdown

      v_tld = p_tld - beta * v;
      y     = M1.solve(v_tld);

      rho_1 = rho;
      rho   = norm(y);
      w_tld = At * q - (beta * w);
      z     = M2t.solve(w_tld);

      xi = norm(z);

      gamma_1 = gamma;
      theta_1 = theta;

      theta = rho / (gamma_1 * beta);
      gamma = 1.0 / sqrt(1.0 + theta * theta);

      if (gamma == 0.0)
        return 4; // return on breakdown

      eta = -eta * rho_1 * gamma * gamma / (beta * gamma_1 * gamma_1);

      if (i > 1)
        {
          d = eta * p + (theta_1 * theta_1 * gamma * gamma) * d;
          s = eta * p_tld + (theta_1 * theta_1 * gamma * gamma) * s;
        }
      else
        {
          d = eta * p;
          s = eta * p_tld;
        }

      x += d; // update approximation vector
      r -= s; // compute residual

      if ((resid = norm(r) / normb) <= tol)
        {
          tol      = resid;
          max_iter = i;
          return 0;
        }
    }

  tol = resid;
  return 1; // no convergence
}
#endif
