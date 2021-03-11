#ifndef HH_BICGSTAB_HH
#define HH_BICGSTAB_HH
//*****************************************************************
// Iterative template routine -- BiCGSTAB
//
// BiCGSTAB solves the unsymmetric linear system Ax = b 
// using the Preconditioned BiConjugate Gradient Stabilized method
//
// BiCGSTAB follows the algorithm described on p. 27 of the 
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
template < class Matrix, class Vector, class Preconditioner>
int 
BiCGSTAB(const Matrix &A, Vector &x, const Vector &b,
         const Preconditioner &M, int &max_iter, typename Vector::Scalar &tol)
{
  using LinearAlgebra::dot;
  using LinearAlgebra::norm;
  using Real= typename Vector::Scalar;

  Real resid;
  Real rho_1(0.), rho_2(0.), alpha(0.), beta(0.), omega(0.);
  Vector p, phat, s, shat, t, v;

  Real normb = norm(b);
  Vector r = b - A * x;
  Vector rtilde = r;

  if (normb == 0.0)
    normb = 1;
  
  if ((resid = norm(r) / normb) <= tol) {
    tol = resid;
    max_iter = 0;
    return 0;
  }

  for (int i = 1; i <= max_iter; i++) {
    rho_1 = dot(rtilde, r);
    if (rho_1 == 0) {
      tol = norm(r) / normb;
      return 2;
    }
    if (i == 1)
      p = r;
    else {
      beta = (rho_1/rho_2) * (alpha/omega);
      p = r + beta * (p - omega * v);
    }
    phat = M.solve(p);
    v = A * phat;
    alpha = rho_1 / dot(rtilde, v);
    s = r - alpha * v;
    if ((resid = norm(s)/normb) < tol) {
      x += alpha * phat;
      tol = resid;
      return 0;
    }
    shat = M.solve(s);
    t = A * shat;
    omega = dot(t,s) / dot(t,t);
    x += alpha * phat + omega * shat;
    r = s - omega * t;

    rho_2 = rho_1;
    if ((resid = norm(r) / normb) < tol) {
      tol = resid;
      max_iter = i;
      return 0;
    }
    if (omega == 0) {
      tol = norm(r) / normb;
      return 3;
    }
  }

  tol = resid;
  return 1;
}
#endif
