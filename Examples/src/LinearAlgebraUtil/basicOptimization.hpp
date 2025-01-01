/*
 * BasicOptimization.hpp
 *
 *  Created on: Dec 23, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_BASICOPTIMIZATION_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_BASICOPTIMIZATION_HPP_
#include <array>
#include <cmath>
#include <limits>
#include <numbers> // C++20 stuff to get golden ration
#include <tuple>
#include <utility>
/*
 * In this version (march 2023) I use concepts to constrain the
 * template parameter indicating the function. Function must be
 * double (double) or double (double const &)
 */
#include "functionConcepts.hpp" // from Utility

namespace apsc
{
/*!
 *  Golden search method to find the extrema of a function. The function must be
 *  double (double) or double (double const &) and must be unimodal in the
 * interval.
 *
 * @tparam Function The type of a callable object double (double)
 * @param f The callable object double (double)
 * @param a the first end of an interval bracketing the extremum
 * @param b the second end of an interval bracketing the extremum
 * @param tol The desired tolerance. It is guaranteed that the found extremum is
 * within the tolerance
 * @param maxIter Max number of iterations. Safeguard against non-convergence in
 * case of floating point errors
 * @return The found extremum and a status (=false max iterations reached)
 */
template <TypeTraits::ScalarFunction Function>
std::tuple<double, bool>
golden_search(Function const &f, double a, double b, double tol = 1.e-5,
              unsigned maxIter = 100)
{
  constexpr double phi =
    1. / std::numbers::phi_v<double>; //(std::sqrt(5.) + 1) / 2;
  unsigned iter{0u};
  auto     c = b - (b - a) * phi;
  auto     d = a + (b - a) * phi;
  auto     fc = f(c);
  auto     fd = f(d);
  tol = std::abs(tol); // just in case
  while(std::abs(b - a) > tol && iter < maxIter)
    {
      ++iter;
      if(f(c) < f(d))
        {
          b = d;
          d = c;
          c = b - (b - a) * phi;
          fd = fc;
          fc = f(c);
        }
      else
        {
          a = c;
          c = d;
          d = a + (b - a) * phi;
          fc = fd;
          fd = f(d);
        }
    }
  // test for convergence: have I found the minimum
  // I estimate the derivative with a finite difference
  bool ok = std::abs(f(c) - f(d)) / std::abs(d - c) < tol;
  return {0.5 * (a + b), ok};
}

/*!
 * Tries to bracket an interval containing a minimum of a continuous function f.
 *
 * @tparam Function The type of a callable object double (double)
 * @param f A callable object of signature double (double)
 * @param x1 An initial guess
 * @param h The initial increment
 * @param maxIter Maximum number of iteration
 * @return The found bracket points and a status (0=ok, 1= bad initial point, 2
 * max iteration exceeded)
 *
 * @note The initial point is not valid if it is near a maximum.
 */
template <TypeTraits::ScalarFunction Function>
std::tuple<double, double, int>
bracketIntervalMinimum(Function const &f, double x1, double h = 0.01,
                       unsigned int maxIter = 100)
{
  constexpr double expandFactor = 1.5;
  int              status = 0;
  // First test
  auto y1 = f(x1);
  // Check monotonicity
  if(f(x1 - h) < y1 and f(x1 + h) > y1)
    h = -h;                             // change direction
  if(f(x1 - h) > y1 and f(x1 + h) > y1) // I have found the bracket
    return {x1 - h, x1 + h, 0};
  bool         isOk{false};
  unsigned int iter{0u};
  auto         x2 = x1;
  do
    {
      h *= expandFactor;
      x2 += h;
      auto y2 = f(x2);
      isOk = (y2 > y1);
      if(!isOk)
        {
          x1 = x2;
          y1 = y2;
        }
      ++iter;
  } while(!isOk and iter < maxIter);
  if(iter >= maxIter)
    status = iter;
  x1 -= h;
  if(x1 > x2)
    std::swap(x1, x2);
  return std::make_tuple(x1, x2, status);
}

//****************************************************************************80
/*
Now a set of routines from the book of Richard Brent, ported to C++ by John
Burkardt. They are an adaptation of two routines found in
https://people.math.sc.edu/Burkardt/cpp_src/brent made by L.Formaggia to port
them to more modern C++ style.

For completeness and acknowledging the original authors I have kept the original
comments, though I have added some doxygen comments as well to allow the
automatic generation of documentation.
*/

//****************************************************************************80
//
//  Purpose:
//
//    GLOMIN seeks a global minimum of a function F(X) in an interval [A,B].
//
//  Discussion:
//
//    This function assumes that F(X) is twice continuously differentiable
//    over [A,B] and that F''(X) <= M for all X in [A,B].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 July 2011
//
//  Author:
//
//    Original FORTRAN77 version by Richard Brent.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Richard Brent,
//    Algorithms for Minimization Without Derivatives,
//    Dover, 2002,
//    ISBN: 0-486-41998-3,
//    LC: QA402.5.B74.
//
//  Parameters:
//
//    Input, double A, B, the endpoints of the interval.
//    It must be the case that A < B.
//
//    Input, double C, an initial guess for the global
//    minimizer.  If no good guess is known, C = A or B is acceptable.
//
//    Input, double M, the bound on the second derivative.
//
//    Input, double E, a positive tolerance, a bound for the
//    absolute error in the evaluation of F(X) for any X in [A,B].
//
//    Input, double T, a positive error tolerance.
//
//    Input, func_base& F, a user-supplied c++ functor whose
//    global minimum is being sought.  The input and output
//    of F() are of type double.
//
//    Output, double &X, the estimated value of the abscissa
//    for which F attains its global minimum value in [A,B].
//
//    Output, double GLOMIN, the value F(X).

/*!
@brief It seeks a global minimum of a function F(X) in an interval [A,B]
@details This function assumes that F(X) is twice continuously differentiable
over [A,B] and that F''(X) <= M for all X in [A,B].
@param a the left endpoint of the interval
@param b the right endpoint of the interval
@param c an initial guess for the global minimizer.  If no good guess is known,
C = A or B is acceptable.
@param m an estimate of the bound on the second derivative. In fact it is an
estimate of the Liptschitz constant of the first derivative
@param e a positive tolerance, a bound for the absolute error in the evaluation
of F(X) for any X in [A,B].
@param t a positive error tolerance.
@param f a user-supplied c++ functor whose global minimum is being sought.  The
input and output of F() are of type double.
@return a tuple containing the estimated value of the abscissa for which F
attains its global minimum value in [A,B] and the value F(X).
@note There is no control on convergence, so no guarantee that the returned
value is indeed a global minimizer.
*/
std::array<double, 2>
Brent_glomin(double a, double b, double c, double m, double e, double t,
             TypeTraits::ScalarFunction auto const &f)

{
  using namespace std;
  double a0;
  double a2;
  double a3;
  double d0;
  double d1;
  double d2;
  double h;
  double x;
  int    k;
  double m2;
  double p;
  double q;
  double qs;
  double r;
  double s;
  double sc;
  double y;
  double y0;
  double y1;
  double y2;
  double y3;
  double yb;
  double z0;
  double z1;
  double z2;

  a0 = b;
  x = a0;
  a2 = a;
  y0 = f(b);
  yb = y0;
  y2 = f(a);
  y = y2;

  if(y0 < y)
    {
      y = y0;
    }
  else
    {
      x = a;
    }

  if(m <= 0.0 || b <= a)
    {
      return {x, y};
    }

  auto constexpr macheps = std::numeric_limits<double>::epsilon();

  m2 = 0.5 * (1.0 + 16.0 * macheps) * m;

  if(c <= a || b <= c)
    {
      sc = 0.5 * (a + b);
    }
  else
    {
      sc = c;
    }

  y1 = f(sc);
  k = 3;
  d0 = a2 - sc;
  h = 9.0 / 11.0;

  if(y1 < y)
    {
      x = sc;
      y = y1;
    }
  //
  //  Loop.
  //
  for(;;)
    {
      d1 = a2 - a0;
      d2 = sc - a0;
      z2 = b - a2;
      z0 = y2 - y1;
      z1 = y2 - y0;
      r = d1 * d1 * z0 - d0 * d0 * z1;
      p = r;
      qs = 2.0 * (d0 * z1 - d1 * z0);
      q = qs;

      if(k < 1000000 || y2 <= y)
        {
          for(;;)
            {
              if(q * (r * (yb - y2) + z2 * q * ((y2 - y) + t)) <
                 z2 * m2 * r * (z2 * q - r))
                {
                  a3 = a2 + r / q;
                  y3 = f(a3);

                  if(y3 < y)
                    {
                      x = a3;
                      y = y3;
                    }
                }
              k = ((1611 * k) % 1048576);
              q = 1.0;
              r = (b - a) * 0.00001 * (double)(k);

              if(z2 <= r)
                {
                  break;
                }
            }
        }
      else
        {
          k = ((1611 * k) % 1048576);
          q = 1.0;
          r = (b - a) * 0.00001 * (double)(k);

          while(r < z2)
            {
              if(q * (r * (yb - y2) + z2 * q * ((y2 - y) + t)) <
                 z2 * m2 * r * (z2 * q - r))
                {
                  a3 = a2 + r / q;
                  y3 = f(a3);

                  if(y3 < y)
                    {
                      x = a3;
                      y = y3;
                    }
                }
              k = ((1611 * k) % 1048576);
              q = 1.0;
              r = (b - a) * 0.00001 * (double)(k);
            }
        }

      r = m2 * d0 * d1 * d2;
      s = sqrt(((y2 - y) + t) / m2);
      h = 0.5 * (1.0 + h);
      p = h * (p + 2.0 * r * s);
      q = q + 0.5 * qs;
      r = -0.5 * (d0 + (z0 + 2.01 * e) / (d0 * m2));

      if(r < s || d0 < 0.0)
        {
          r = a2 + s;
        }
      else
        {
          r = a2 + r;
        }

      if(0.0 < p * q)
        {
          a3 = a2 + p / q;
        }
      else
        {
          a3 = r;
        }

      for(;;)
        {
          a3 = std::max(a3, r);

          if(b <= a3)
            {
              a3 = b;
              y3 = yb;
            }
          else
            {
              y3 = f(a3);
            }

          if(y3 < y)
            {
              x = a3;
              y = y3;
            }

          d0 = a3 - a2;

          if(a3 <= r)
            {
              break;
            }

          p = 2.0 * (y2 - y3) / (m * d0);

          if((1.0 + 9.0 * macheps) * d0 <= fabs(p))
            {
              break;
            }

          if(0.5 * m2 * (d0 * d0 + p * p) <= (y2 - y) + (y3 - y) + 2.0 * t)
            {
              break;
            }
          a3 = 0.5 * (a2 + a3);
          h = 0.9 * h;
        }

      if(b <= a3)
        {
          break;
        }

      a0 = sc;
      sc = a2;
      a2 = a3;
      y0 = y1;
      y1 = y2;
      y2 = y3;
    }

  return {x, y};
}
//****************************************************************************80

//****************************************************************************80
//
//  Purpose:
//
//    LOCAL_MIN seeks a local minimum of a function F(X) in an interval [A,B].
//
//  Discussion:
//
//    The method used is a combination of golden section search and
//    successive parabolic interpolation.  Convergence is never much slower
//    than that for a Fibonacci search.  If F has a continuous second
//    derivative which is positive at the minimum (which is not at A or
//    B), then convergence is superlinear, and usually of the order of
//    about 1.324....
//
//    The values EPS and T define a tolerance TOL = EPS * abs ( X ) + T.
//    F is never evaluated at two points closer than TOL.
//
//    If F is a unimodal function and the computed values of F are always
//    unimodal when separated by at least SQEPS * abs ( X ) + (T/3), then
//    LOCAL_MIN approximates the abscissa of the global minimum of F on the
//    interval [A,B] with an error less than 3*SQEPS*abs(LOCAL_MIN)+T.
//
//    If F is not unimodal, then LOCAL_MIN may approximate a local, but
//    perhaps non-global, minimum to the same accuracy.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 July 2011
//
//  Author:
//
//    Original FORTRAN77 version by Richard Brent.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Richard Brent,
//    Algorithms for Minimization Without Derivatives,
//    Dover, 2002,
//    ISBN: 0-486-41998-3,
//    LC: QA402.5.B74.
//
//  Parameters:
//
//    Input, double A, B, the endpoints of the interval.
//
//    Input, double T, a positive absolute error tolerance.
//
//    Input, func_base& F, a user-supplied c++ functor whose
//    local minimum is being sought.  The input and output
//    of F() are of type double.
//
//    Output, double &X, the estimated value of an abscissa
//    for which F attains a local minimum value in [A,B].
//
//    Output, double LOCAL_MIN, the value F(X).
//

/*!
@brief It seeks a local minimum of a function F(X) in an interval [A,B]

@details The method used is a combination of golden section search and
successive parabolic interpolation.  Convergence is never much slower than that
for a Fibonacci search.  If F has a continuous second derivative which is
positive at the minimum (which is not at A or B  then convergence is
superlinear, and usually of the order of about 1.324....

@param a the left endpoint of the interval
@param b the right endpoint of the interval
@param t a positive absolute error tolerance.
@param f a user-supplied c++ functor whose local minimum is being sought.  The
input and output of F() are of type double.
@return a tuple containing the estimated value of an abscissa for which F
attains a local minimum value in [A,B] and the value F(X).
@note No convergence test is made.  The routine assumes that the minimum lies
between A and B and that F(A) > F(X) < F(B).
*/
std::array<double, 2>
Brent_local_min(double a, double b, double t,
                TypeTraits::ScalarFunction auto const &f)
{
  using namespace std;
  double d;
  double e;
  double fu;
  double fv;
  double fw;
  double fx;
  double m;
  double p;
  double q;
  double r;
  double sa;
  double sb;
  double t2;
  double tol;
  double u;
  double v;
  double w;
  double x;
  //
  //  C is the square of the inverse of the golden ratio.
  //
  double constexpr c =
    1. / (std::numbers::phi_v<double> * std::numbers::phi_v<double>);
  // c = 0.5 * ( 3.0 - sqrt ( 5.0 ) ); (sqrt is not constexpr)

  const double eps = sqrt(std::numeric_limits<double>::epsilon());

  sa = a;
  sb = b;
  x = sa + c * (b - a);
  w = x;
  v = w;
  e = 0.0;
  fx = f(x);
  fw = fx;
  fv = fw;

  for(;;)
    {
      m = 0.5 * (sa + sb);
      tol = eps * fabs(x) + t;
      t2 = 2.0 * tol;
      //
      //  Check the stopping criterion.
      //
      if(fabs(x - m) <= t2 - 0.5 * (sb - sa))
        {
          break;
        }
      //
      //  Fit a parabola.
      //
      r = 0.0;
      q = r;
      p = q;

      if(tol < fabs(e))
        {
          r = (x - w) * (fx - fv);
          q = (x - v) * (fx - fw);
          p = (x - v) * q - (x - w) * r;
          q = 2.0 * (q - r);
          if(0.0 < q)
            {
              p = -p;
            }
          q = fabs(q);
          r = e;
          e = d;
        }

      if(fabs(p) < fabs(0.5 * q * r) && q * (sa - x) < p && p < q * (sb - x))
        {
          //
          //  Take the parabolic interpolation step.
          //
          d = p / q;
          u = x + d;
          //
          //  F must not be evaluated too close to A or B.
          //
          if((u - sa) < t2 || (sb - u) < t2)
            {
              if(x < m)
                {
                  d = tol;
                }
              else
                {
                  d = -tol;
                }
            }
        }
      //
      //  A golden-section step.
      //
      else
        {
          if(x < m)
            {
              e = sb - x;
            }
          else
            {
              e = sa - x;
            }
          d = c * e;
        }
      //
      //  F must not be evaluated too close to X.
      //
      if(tol <= fabs(d))
        {
          u = x + d;
        }
      else if(0.0 < d)
        {
          u = x + tol;
        }
      else
        {
          u = x - tol;
        }

      fu = f(u);
      //
      //  Update A, B, V, W, and X.
      //
      if(fu <= fx)
        {
          if(u < x)
            {
              sb = x;
            }
          else
            {
              sa = x;
            }
          v = w;
          fv = fw;
          w = x;
          fw = fx;
          x = u;
          fx = fu;
        }
      else
        {
          if(u < x)
            {
              sa = u;
            }
          else
            {
              sb = u;
            }

          if(fu <= fw || w == x)
            {
              v = w;
              fv = fw;
              w = u;
              fw = fu;
            }
          else if(fu <= fv || v == x || v == w)
            {
              v = u;
              fv = fu;
            }
        }
    }
  return {x, fx};
}

} // namespace apsc

#endif /* EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_BASICOPTIMIZATION_HPP_ */
