/*
 * secant.hpp
 *
 *  Created on: Nov 20, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_BASICZEROFUN_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_BASICZEROFUN_HPP_
#include "extendedAssert.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <tuple>
namespace apsc
{
/*!
 * Computes the zero of a scalar function with the method of the chord
 * We stop when the residual is below lolerance;
 *
 * @tparam Function
 * @param f The function
 * @param a First end of initial interval
 * @param b Second end of initial interval
 * @param tol Tolerance (relative)
 * @parma tola Tolerance (absolute)
 * @return The approximation of the zero of f
 * @pre f(a)*f(b)<0
 */
template <class Function>
double
chord(Function const &f, double a, double b, double tol=1.e-6, double tola=1.e-10)
{
  double ya = f(a);
  double yb = f(b);
  double delta = b - a;
  double resid0 = std::max(std::abs(ya),std::abs(yb));
  SURE_ASSERT(ya * yb < 0, "Function must change sign at the two end values");
  double           yc{ya};
  double           c{a};
  double           incr = std::numeric_limits<double>::max();
  constexpr double small = 10.0 * std::numeric_limits<double>::epsilon();
  while (std::abs(yc) > tol*resid0+tola && incr > small)
    {
      double incra = -ya/ (yb - ya);
      double incrb = 1.-incra;
      double incr = std::min(incra,incrb);
      ASSERTM((std::max(incra,incrb)<=1.0 && incr>=0), "Chord is failing")
      c = a + incra*delta;
      //std::cout << c << std::endl;
      yc = f(c);
      if (yc * ya < 0.0)
        {
          yb = yc;
          b = c;
        }
      else
        {
          ya = yc;
          a = c;
        }
      delta = b - a;
/*
 *      std::cout << delta << " " << yc << " " << ya << " " << yb << " " << incr
 *      << std::endl;
 */
    }
  return c;
}

/*!
 * Computes the zero of a scalar function with the method of the bisection
 * The returned value is far from the zero at most the given tolerance;
 *
 * @tparam Function
 * @param f The function
 * @param a First end of initial interval
 * @param b Second end of initial interval
 * @param tol Tolerance
 * @return The approximation of the zero of f
 * @pre f(a)*f(b)<0
 */
template <class Function>
double
bisection(Function const &f, double a, double b, double tol)
{
  double ya = f(a);
  double yb = f(b);
  double delta = b - a;
  SURE_ASSERT(ya * yb < 0, "Function must change sign at the two end values");
  double yc{ya};
  double c{a};
  while (std::abs(delta) > 2 * tol)
    {
      c = (a + b) / 2.;
      yc = f(c);
      if (yc * ya < 0.0)
        {
          yb = yc;
          b = c;
        }
      else
        {
          ya = yc;
          a = c;
        }
      delta = b - a;
    }
  return c;
}
/*!
 * Computes the zero of a scalar function with the method of the secant
 * It stops when \f$|f(x)|\le tol|f(a)| + tola
 *
 * @tparam Function
 * @param f The function
 * @param a First point for computation of derivatives
 * @param b Second point for computing derivatives
 * @param tol relative tolerance
 * @param tola absolute tolerance
 * @param maxIt maximum number of iterations
 * @return The approximation of the zero of f and a status (false if not
 * converging)
 *
 */
template <class Function>
std::tuple<double, bool>
secant(Function const &f, double a, double b, double tol = 1e-4,
       double tola = 1.e-10, unsigned int maxIt = 150)
{
  double       ya = f(a);
  double       resid = std::abs(ya);
  double       c{a};
  unsigned int iter{0u};
  double       check = tol * resid + tola;
  while (resid > check && iter < maxIt)
    {
      ++iter;
      double yb = f(b);
      c = a - ya * (b - a) / (yb - ya);
      double yc = f(c);
      resid = std::abs(yc);
      if (resid < std::abs(ya))
        {
          yb = yc;
          b = c;
        }
      else
        {
          ya = yc;
          a = c;
        }
    }
  return std::make_tuple(c, iter < maxIt);
}

} // end namespace

#endif /* EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_BASICZEROFUN_HPP_ */
