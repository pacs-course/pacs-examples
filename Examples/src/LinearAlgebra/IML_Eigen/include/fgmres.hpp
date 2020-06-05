#ifndef HH_FGMRES_HH
#define HH_FGMRES_HH

//*****************************************************************
//! Iterative template routine -- FGMRES
//!
//! FGMRES solves the unsymmetric linear system Ax = b using the
//! Flexible version for Generalized Minimum Residual method
//!
//! FGMRES(m) follows the algorithm described in
//! "A flexible inner-outer preconditioned GMRES algorithm"
//!  by Y. Saad
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
FGMRES (const Matrix &A, Vector &x, const Vector &b, const Preconditioner &M,
        int &m, int &max_iter, typename Vector::Scalar &tol)
{
  using LinearAlgebra::dot;
  using LinearAlgebra::norm;
  using Real = typename Vector::Scalar;
  Real   resid;
  int    i, j = 1, k;
  Vector s (m + 1), cs (m + 1), sn (m + 1);
  Vector w (b.size ());
  Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> H =
    Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>::Zero (m + 1, m);
  //  H.resize(m+1,m,0.0);
  //  Real normb = norm(M.solve(b));
  Real   normb = norm (b);
  Vector r = b - A * x;
  Real   beta = norm (r);

  if (normb == 0.0)
    normb = 1;
  resid = beta / normb;
  if (resid <= tol)
    {
      tol = resid;
      max_iter = 0;
      return 0;
    }

  std::vector<Vector> z (m + 1, Vector (b.size ()));
  std::vector<Vector> v (m + 1, Vector (b.size ()));

  while (j <= max_iter)
    {
      v[0] = (1.0 / beta) * r;
      s = Vector::Zero (m + 1);
      s (0) = beta;

      for (i = 0; i < m && j <= max_iter; i++, j++)
        {
          z[i] = M.solve (v[i]); // z
          w = A * z[i];
          for (k = 0; k <= i; k++)
            {
              H (k, i) = dot (w, v[k]);
              w -= H (k, i) * v[k];
            }
          H (i + 1, i) = norm (w);
          v[i + 1] = w * (1.0 / H (i + 1, i)); //

          for (k = 0; k < i; k++)
            ApplyPlaneRotation (H (k, i), H (k + 1, i), cs (k), sn (k));

          GeneratePlaneRotation (H (i, i), H (i + 1, i), cs (i), sn (i));
          ApplyPlaneRotation (H (i, i), H (i + 1, i), cs (i), sn (i));
          ApplyPlaneRotation (s (i), s (i + 1), cs (i), sn (i));

          if ((resid = std::abs (s (i + 1)) / normb) < tol)
            {
              Update (x, i, H, s, z);
              tol = resid;
              max_iter = j;
              return 0;
            }
        }
      Update (x, m - 1, H, s, z);
      r = b - A * x;
      beta = norm (r);
//      resid = beta / normb;
//      if (resid < tol)
//        {
//          tol = resid;
//          max_iter = j;
//          return 0;
//        }
    }
  tol = resid;
  return 1;
}
} // end neamespace
#endif
