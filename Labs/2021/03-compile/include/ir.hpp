#ifndef HH_IRLINALG_HH
#define HH_IRLINALG_HH
//*****************************************************************
// Iterative template routine -- Preconditioned Richardson
//
// IR solves the unsymmetric linear system Ax = b using
// Iterative Refinement (preconditioned Richardson iteration).
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
IR(const Matrix &           A,
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
  Vector z;

  Real   normb = norm(b);
  Vector r     = b - A * x;

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
      z = M.solve(r);
      x += z;
      r = b - A * x;

      if ((resid = norm(r) / normb) <= tol)
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
