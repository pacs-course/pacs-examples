#ifndef HH_BICG__HH
#define HH_BICG__HH
//*****************************************************************
// Iterative template routine -- BiCG
//
// BiCG solves the unsymmetric linear system Ax = b
// using the Preconditioned BiConjugate Gradient method
//
// BiCG follows the algorithm described on p. 22 of the
// SIAM Templates book.
//
// The return value indicates convergence within max_iter (input)
// iterations (0), or no convergence within max_iter iterations (1).
//
// Upon successful return, output arguments have the following values:
//
//        x  --  approximate solution to Ax = b
// max_iter  --  the number of iterations performed before the
//               tolerance was reached
//      tol  --  the residual after the final iteration
//
//*****************************************************************
#include "LinearAlgebraTraits.hpp"

template <class Matrix, class Vector, class Preconditioner>
int
BiCG(const Matrix &           A,
     Vector &                 x,
     const Vector &           b,
     const Preconditioner &   M,
     int &                    max_iter,
     typename Vector::Scalar &tol)
{
  using LinearAlgebra::dot;
  using LinearAlgebra::norm;
  using Real = typename Vector::Scalar;

  Real   resid;
  Real   rho_1(0.), rho_2(0.), alpha(0.), beta(0.);
  Vector z, ztilde, p, ptilde, q, qtilde;

  Real   normb  = norm(b);
  Vector r      = b - A * x;
  Vector rtilde = r;

  // I need left preconditioner.

  Matrix         At = A.transpose();
  Preconditioner Mt(At);

  if (normb == 0.0)
    normb = 1;

  if ((resid = norm(r) / normb) <= tol)
    {
      tol      = resid;
      max_iter = 0;
      return 0;
    }

  for (int i = 1; i <= max_iter; i++)
    {
      z      = M.solve(r);
      ztilde = Mt.solve(rtilde);
      rho_1  = dot(z, rtilde);
      if (rho_1 == 0)
        {
          tol      = norm(r) / normb;
          max_iter = i;
          return 2;
        }
      if (i == 1)
        {
          p      = z;
          ptilde = ztilde;
        }
      else
        {
          beta   = rho_1 / rho_2;
          p      = z + beta * p;
          ptilde = ztilde + beta * ptilde;
        }
      q      = A * p;
      qtilde = At * ptilde;
      alpha  = rho_1 / dot(ptilde, q);
      x += alpha * p;
      r -= alpha * q;
      rtilde -= alpha * qtilde;

      rho_2 = rho_1;
      if ((resid = norm(r) / normb) < tol)
        {
          tol      = resid;
          max_iter = i;
          return 0;
        }
    }

  tol = resid;
  return 1;
}

#endif
