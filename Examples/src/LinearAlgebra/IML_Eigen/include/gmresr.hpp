#ifndef HH_GMRESR_HH
#define HH_GMRESR_HH

//*****************************************************************
//! Iterative template routine -- GMRES
//!
//! GMRES solves the unsymmetric linear system Ax = b using the
//! Generalized Minimum Residual method preconditioned with a right
//! preconditioner
//!
//! GMRES(m) follows the algorithm described on p. 20 of the
//! SIAM Templates book.
//!
//! The return value indicates convergence within max_iter (input)
//! iterations (0), or no convergence within max_iter iterations (1).
//
//! Upon successful return, output arguments have the following values:
//!
//!    \param Matrix  An Eigen sparse matrix
//!    \param Vector  An Eigen vector
//!    \param Preconditioner Something that obeys the preconditioner concept: It
//!    has a method called solve(Vector const&). \param b The right hand side
//!    \param x approximate solution to Ax = b (initial guess at te start)
//!    \param max_iter the number of iterations performed before the
//!               tolerance was reached
//!    \param m  The restart level.
//!    \param tol the residual after the final iteration

//*****************************************************************
#include "LinearAlgebraTraits.hpp"
#include "gmres_util.hpp"
#include <Eigen/Dense>
#include <memory>
namespace LinearAlgebra
{
template <class Matrix, class Vector, class Preconditioner>
int
GMRESR (const Matrix &A, Vector &x, const Vector &b, const Preconditioner &M,
        int &m, int &max_iter, typename Vector::Scalar &tol)
{
  using LinearAlgebra::dot;
  using LinearAlgebra::norm;
  using Real = typename Vector::Scalar;
  Real   resid;
  int    i, j = 1, k;
  Vector s (m + 1), cs (m + 1), sn (m + 1);
  Vector w (b.size ());
  Vector z (b.size ());
  Vector y (b.size ());
  Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> H =
    Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>::Zero (m + 1, m);
  //  H.resize(m+1,m,0.0);
  Real   normb = norm (b);
  Vector r = b - A * x;
  y = x; // it should be M*x. Better set x=0
  Real beta = norm (r);

  if (normb == 0.0)
    normb = 1;
  resid = norm (r) / normb;
  if (resid <= tol)
    {
      tol = resid;
      max_iter = 0;
      return 0;
    }

  std::vector<Vector> v (m + 1, Vector (b.size ()));

  while (j <= max_iter)
    {
      v[0] = r * (1.0 / beta); // ??? r / beta
      s = Vector::Zero (m + 1);
      s (0) = beta;

      for (i = 0; i < m && j <= max_iter; i++, j++)
        {
          z = M.solve (v[i]);
          w = A * z;
          for (k = 0; k <= i; k++)
            {
              H (k, i) = dot (w, v[k]);
              w -= H (k, i) * v[k];
            }
          H (i + 1, i) = norm (w);
          v[i + 1] = w * (1.0 / H (i + 1, i)); // ??? w / H(i+1, i)

          for (k = 0; k < i; k++)
            ApplyPlaneRotation (H (k, i), H (k + 1, i), cs (k), sn (k));

          GeneratePlaneRotation (H (i, i), H (i + 1, i), cs (i), sn (i));
          ApplyPlaneRotation (H (i, i), H (i + 1, i), cs (i), sn (i));
          ApplyPlaneRotation (s (i), s (i + 1), cs (i), sn (i));

          if ((resid = std::abs (s (i + 1)) / normb) < tol)
            {
              Update (y, i, H, s, v);
              x = M.solve (y);
              tol = resid;
              max_iter = j;
              return 0;
            }
        }
      Update (y, m - 1, H, s, v);
      x = M.solve (y);
      r = b - A * x;
      beta = norm (r);
      resid = norm (w) / beta;
      if (resid < tol)
        {
          tol = resid;
          max_iter = j;
          return -1;
        }
    }

  tol = resid;
  return 1;
}
}

#endif
