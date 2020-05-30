// tminres is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License (as published by the Free
// Software Foundation) version 2.1 dated February 1999.
//
// Authors:
// - Umberto Villa, Emory University - uvilla@emory.edu
// - Michael Saunders, Stanford University
// - Santiago Akle, Stanford University
// - modified by Luca Formaggia Politecnico di Milano

/*!
  @file
  @author U. Villa - uvilla@emory.edu
  @date 04/2012

  @fn int TMINRES(const Operator &A, Vector &x, const Vector &b,
  const Preconditioner * M,  int &max_iter, double &tol, double shift,bool show)

  @brief Preconditioner Minres algorithm

  minres solves the n x n system of linear equations Ax = b
  or the n x n least squares problem           min ||Ax - b||_2^2,
  where A is a symmetric matrix (possibly indefinite or singular)
  and b is a given vector.  The dimension n is defined by length(b).

  @param A const OPERATOR & (INPUT)
  A is a linear operator with a method Apply(const Vector & X, Vector & Y)
  which gives the product y = A*x for any given n-vector x.

  @param x VECTOR & (INPUT/OUTPUT)
  "x" is the initial guess (INPUT) and the solution of the linear system
  (OUTPUT).

  @param b const VECTOR & (INPUT)
  "b" is the right hand side of the linear system

  @param M const PRECONDITIONER * (INPUT)
  "M" is a linear operator (preconditioner) with a method Apply(const Vector &
  X, Vector & Y) such that y = M(x) solves the system My = x given any n-vector
  x. If M is a null pointer, preconditioning is not used.  Otherwise, "M"
  defines a (implicitely) a positive-definite preconditioner M = C*C'.

  @param shift double (INPUT)
  If shift != 0, minres really solves (A - shift*I)x = b
  (or the corresponding least-squares problem if shift is an
  eigenvalue of A).

  @param max_iter int& (INPUT/OUTPUT)
  "max_iter" is the maximum number of minres iterations (INPUT) and the actual
  number of iteration at termination (OUTPUT)

  @param tol double& (INPUT)
  "tol" is tolerance for the stopping criterion

  @param show bool (INPUT)
  If "show" is true print information on screen.

  @return istop int
  "istop"  is a value from [-1:9] to indicate the reason for termination.
  The reason is summarized in msg[istop+2] below.

  When M = C*C' exists, minres implicitly solves the system

  @verbatim
  P(A - shift*I)P'xbar = Pb,
  i.e.               Abar xbar = bbar,
  where                      P = inv(C),
  Abar = P(A - shift*I)P',
  bbar = Pb,

  and returns the solution      x = P'xbar.
  The associated residual is rbar = bbar - Abar xbar
  = P(b - (A - shift*I)x)
  = Pr.
  @endverbatim

  Known bugs:
  <ul>
  <li> As Jeff Kline pointed out, Arnorm = ||A r_{k-1}|| lags behind
  rnorm = ||r_k||.  On singular systems, this means that a good
  least-squares solution exists before Arnorm is small enough
  to recognize it.  The solution x_{k-1} gets updated to x_k
  (possibly a very large solution) before Arnorm shuts things
  down the next iteration.  It would be better to keep x_{k-1}.
  </ul>

  Other notes:
  <ul>
  <li> ynorm = norm(x) is computed directly at the cost of an additional
  inner product. This can represent a bottle-neck in the parallel case.
  In fact it is possible to avoid norm(x) by updating certain scalar quantities
  (see the QLP factors in MINRES-QLP).  It would add only cheap (scalar)
  arithmetic, but lots of extra code.
  </ul>
*/

#ifndef HH__LINALGTMINRES__HH
#define HH__LINALGTMINRES__HH
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
namespace LinearAlgebra
{
template <typename Operator, typename Vector, typename Preconditioner>
int
TMINRES (const Operator &A, Vector &x, const Vector &b, const Preconditioner &M,
         int &max_iter, double &tol, double shift = 0.0, bool show = false)
{
  double eps (std::numeric_limits<double>::epsilon ());

  std::vector<std::string> msg (11);
  msg[0] = " beta1 = 0.  The exact solution is  x = 0 ";
  msg[1] = " A solution to Ax = b was found, given tol ";
  msg[2] = " A least-squares solution was found, given tol ";
  msg[3] = " Reasonable accuracy achieved, given eps ";
  msg[4] = " x has converged to an eigenvector ";
  msg[5] = " acond has exceeded 0.1/eps ";
  msg[6] = " The iteration limit was reached ";
  msg[7] = " A  does not define a symmetric matrix ";
  msg[8] = " M  does not define a symmetric matrix ";
  msg[9] = " M  does not define a pos-def preconditioner ";
  msg[10] = " beta2 = 0.  If M = I, b and x are eigenvectors ";

  if (show)
    {
      std::cout << std::setfill ('-') << std::setw (80) << "-"
                << "\n";
      std::cout << "|            Adapted from minres.m, Stanford University, "
                   "06 Jul 2009            |\n";
      std::cout << "|                Solution of symmetric Ax=b or "
                   "(A-shift*I)x = b                 |\n";
      std::cout << std::setfill ('-') << std::setw (80) << "-"
                << "\n";
      std::cout << std::setfill (' ');
      std::cout << "shift = " << shift << "; tol = " << tol
                << "; max iter = " << max_iter << "\n";
    }

  int    istop (0), itn (0);
  double Anorm (0.0), Acond (0.0), Arnorm (0.0);
  double rnorm (0.0), ynorm (0.0);
  bool   done (false);

  // Step 1
  /*
   * Set up y and v for the first Lanczos vector v1.
   * y = beta1 P' v1, where P = C^(-1).
   * v is really P'v1
   */

  Vector r1 (x.size ());
  Vector y (x.size ());

  r1 = b - A * x + shift * x;
  y = M.solve (r1);
  double beta1 = r1.dot (y);

  // Test for an indefined preconditioner
  // If b = 0 exactly stop with x = x0.

  if (beta1 < 0.0)
    {
      istop = 9;
      show = true;
      done = true;
    }
  else
    {
      if (beta1 == 0.0)
        {
          show = true;
          done = true;
        }
      else
        beta1 = std::sqrt (beta1); // Normalize y to get v1 later
    }

  // STEP 2
  /* Initialize other quantities */
  double oldb (1.0), beta (beta1), dbar (0.0), epsln (0.0), oldeps (0.0);
  double qrnorm (beta1), phi (0.0), phibar (beta1), rhs1 (beta1);
  double rhs2 (0.0), tnorm2 (0.0), ynorm2 (0.0);
  double cs (-1.0), sn (0.0);
  double gmax (0.0), gmin (std::numeric_limits<double>::max ());
  double alpha (0.0), gamma (0.0);
  double delta (0.0), gbar (0.0);
  double z (0.0);

  Vector w (x.size ());
  w.setZero ();
  Vector w1 (x.size ());
  w1.setZero ();
  Vector w2 (x.size ());
  w2.setZero ();
  Vector r2 (x.size ());
  r2 = r1;
  Vector v (x.size ());

  if (show)
    std::cout << std::setw (6) << "Itn" << std::setw (14) << "Compatible"
              << std::setw (14) << "LS" << std::setw (14) << "norm(A)"
              << std::setw (14) << "cond(A)" << std::setw (14) << "gbar/|A|"
              << "\n";

  /* Main Iteration */
  if (!done)
    {
      for (itn = 0; itn < max_iter; ++itn)
        {
          // STEP 3
          /*
            -----------------------------------------------------------------
            Obtain quantities for the next Lanczos vector vk+1, k = 1, 2,...
            The general iteration is similar to the case k = 1 with v0 = 0:

            p1      = Operator * v1  -  beta1 * v0,
            alpha1  = v1'p1,
            q2      = p2  -  alpha1 * v1,
            beta2^2 = q2'q2,
            v2      = (1/beta2) q2.

            Again, y = betak P vk,  where  P = C**(-1).
            .... more description needed.
            -----------------------------------------------------------------
          */
          double s = 1. / beta; // Normalize previous vector (in y)
          v = y * s;

          y = A * v - shift * v;
          if (itn)
            y -= r1 * beta / oldb;

          alpha = v.dot (y); // alphak
          y -= r2 * alpha / beta;
          r1 = r2;
          r2 = y;
          y = M.solve (r2);
          oldb = beta; // oldb = betak
          double beta2 = r2.dot (y);

          if (beta2 < 0)
            {
              istop = 9;
              break;
            }

          tnorm2 += alpha * alpha + oldb * oldb + beta2;
          beta = sqrt (beta2);

          if (itn == 0) // Initialize a few things
            {
              if (beta / beta1 < 10.0 * eps)
                istop = 10;
            }

          // Apply previous rotation Q_{k-1} to get
          // [delta_k epsln_{k+1}] = [cs sn]  [dbar_k 0]
          // [gbar_k   dbar_{k+1}]   [sn -cs] [alpha_k beta_{k+1}].
          oldeps = epsln;
          delta = cs * dbar + sn * alpha;
          gbar = sn * dbar - cs * alpha;
          epsln = sn * beta;
          dbar = -cs * beta;
          double root = sqrt (gbar * gbar + dbar * dbar);
          Arnorm = phibar * root; // ||Ar_{k-1}||

          // Compute next plane rotation Q_k
          gamma = sqrt (gbar * gbar + beta2); // gamma_k
          gamma = std::max (gamma, eps);
          cs = gbar / gamma;    // c_k
          sn = beta / gamma;    // s_k
          phi = cs * phibar;    // phi_k
          phibar = sn * phibar; // phibar_{k+1}

          // Update x
          double denom (1. / gamma);
          w1 = w2;
          w2 = w;
          w = (-oldeps * w1 - delta * w2 + v) * denom;
          x += phi * w;
          // go round again
          gmax = std::max (gmax, gamma);
          gmin = std::min (gmin, gamma);
          z = rhs1 / gamma;
          rhs1 = rhs2 - delta * z;
          rhs2 = -epsln * z;

          // Estimate various norms

          Anorm = sqrt (tnorm2);
          ynorm2 = x.dot (x);
          ynorm = sqrt (ynorm2);
          double epsa (Anorm * eps);
          double epsx (epsa * ynorm);
          // double epsr(Anorm*ynorm*tol);
          qrnorm = phibar;
          rnorm = qrnorm;
          double test1 = rnorm / (Anorm * ynorm); // ||r||/(||A|| ||x||);
          double test2 = root / Anorm; // ||A r_{k-1}|| / (||A|| ||r_{k-1}||)

          // Estimate cond(A)
          /*
            In this version we look at the diagonals of  R  in the
            factorization of the lower Hessenberg matrix,  Q * H = R,
            where H is the tridiagonal matrix from Lanczos with one
            extra row, beta(k+1) e_k^T.
          */
          Acond = gmax / gmin;

          // See if any of the stopping criteria is satisfied
          if (0 == istop)
            {
              double t1 (1.0 + test1),
                t2 (1.0 + test2); // This test work if tol < eps
              if (t2 <= 1.)
                istop = 2;
              if (t1 <= 1.)
                istop = 1;

              if (itn >= max_iter - 1)
                istop = 6;
              if (Acond >= .1 / eps)
                istop = 4;
              if (epsx >= beta1)
                istop = 3;
              if (test2 <= tol)
                istop = 2;
              if (test1 <= tol)
                istop = 1;
            }

          if (show)
            std::cout << std::setw (6) << itn << std::setw (14) << test1
                      << std::setw (14) << test2 << std::setw (14) << Anorm
                      << std::setw (14) << Acond << std::setw (14)
                      << gbar / Anorm << std::endl;

          if (0 != istop)
            break;
        }
    }

  // Display final status
  if (show)
    {
      std::cout << std::setfill ('-') << std::setw (80) << "-"
                << "\n";
      std::cout << msg[istop] << "\n";
      std::cout << " Number of iterations: " << itn << "\n";
      std::cout << " Anorm = " << Anorm << "\t Acond = " << Acond << "\n";
      std::cout << " rnorm = " << rnorm << "\t ynorm = " << ynorm << "\n";
      std::cout << " Arnorm = " << Arnorm << "\n";
      std::cout << std::setfill ('-') << std::setw (80) << "-" << std::endl;
      std::cout << std::setfill (' ');
    }
  max_iter = itn;
  tol = rnorm;
  return istop;
}
} // end namespace
#endif
