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
#include <algorithm>
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
chord(Function const &f, double a, double b, double tol = 1.e-6,
      double tola = 1.e-10)
{
  double ya = f(a);
  double yb = f(b);
  double delta = b - a;
  double resid0 = std::max(std::abs(ya), std::abs(yb));
  SURE_ASSERT(ya * yb < 0, "Function must change sign at the two end values");
  double           yc{ya};
  double           c{a};
  double           incr = std::numeric_limits<double>::max();
  constexpr double small = 10.0 * std::numeric_limits<double>::epsilon();
  while (std::abs(yc) > tol * resid0 + tola && incr > small)
    {
      double incra = -ya / (yb - ya);
      double incrb = 1. - incra;
      double incr = std::min(incra, incrb);
      ASSERTM((std::max(incra, incrb) <= 1.0 && incr >= 0), "Chord is failing")
      c = a + incra * delta;
      // std::cout << c << std::endl;
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
       *      std::cout << delta << " " << yc << " " << ya << " " << yb << " "
       * << incr
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
 * @note It the interval brackets the zero convergence should be guaranteed.
 *
 */
template <class Function>
double
bisection(Function const &f, double a, double b, double tol=1.e-5)
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
  return (a+b)/2.;
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
  bool goOn=resid>check;
  while (goOn && iter < maxIt)
    {
      goOn=resid>check;
      ++iter;
      double yb = f(b);
      if(std::abs(b)>std::abs(a))
        {
          std::swap(a,b);
          std::swap(ya,yb);
        }
      c = a - ya * (b - a) / (yb - ya);
      double yc = f(c);
      resid = std::abs(yc);
      ya = yc;
      a  = c;
    }

  return std::make_tuple(c, iter < maxIt);
}

/*!
 * This function tries to find an interval that brackets the zero of a
 * function f. It does so by sampling the value of f at points
 * generated starting from a given point
 *
 * @tparam Function the type of the function. must be convertible to double
 * (*)(double)
 * @param f The function.
 * @param x1 initial point
 * @param h initial increment for the sampling
 * @param maxIter maximum number of iterations
 * @return a tuple with the bracketing points and a bool which is true if number
 * of iterations not exceeded (bracket found)
 * @note I am adopting a simple algorithm. The search can be bettered using a
 * quadratic (or cubic) search.
 */
template <class Function>
std::tuple<double, double, bool>
bracketInterval(Function const &f, double x1, double h = 0.01,
                unsigned int maxIter = 200)
{
  constexpr double expandFactor = 1.5;
  h=std::abs(h);
  //auto          hinit = h;
  auto          direction=1.0;
  auto             x2 = x1 + h;
  auto             y1 = f(x1);
  auto             y2 = f(x2);
  unsigned int     iter = 0u;
  // get initial decrement direction
  while ((y1 * y2 > 0) && (iter < maxIter))
    {
      ++iter;
      if (std::abs(y2) > std::abs(y1))
        {
          std::swap(y1,y2);
          std::swap(x1,x2);
          // change direction
        }
      direction=(y2>=0&&y1>=0)?-1.0:1.0;
      x1 = x2;
      y1 = y2;
      x2 +=direction*h;
      y2 = f(x2);
      h*=expandFactor;
    }
  return std::make_tuple(x1, x2, iter <= maxIter);
}
/*!
 * Brent type search
 * If converging, it finds a zero with error below the given tolerance.
 *
 * @tparam Function The type of a callable object double (double)
 * @param f A callable object double (double)
 * @param a The first end of a bracketing interval
 * @param b The second end of a bracketing interval
 * @param tol Tolerance
 * @param maxIter Max number of iteration.
 * @return The found approximated zero and a status flag (true=converged).
 *
 * @pre f(a)*f(b)<0
 * @note It the interval brackets the zero convergence should be guaranteed.
 */
template <class Function>
std::tuple<double,bool> brent_search(const Function & f, double a, double b, double tol=1.e-5, unsigned maxIter=200)
{
  auto ya = f(a);
  auto yb = f(b);
  // First check.
  if ((ya*yb) >= 0.0)
    {
      if (ya==0.)
        return {a,true};
      else if (yb==0.)
        return {b,true};
      else
        return {a,false}; // precondition not met
    };
  //
  if(std::abs(ya)<std::abs(yb))
    {
      std::swap(a,b);
      std::swap(ya,yb);
    }
  //
    auto c  = a;
    auto d  = c;
    auto yc = ya;
    bool mflag{true};
    auto   s=b;
    auto   ys=yb;
    unsigned iter{0u};
    do
      {
//
        if(ya != yc and yb != yc)
          {
            auto yab = ya-yb;
            auto yac = ya-yc;
            auto ycb = yc-yb;
            // inverse quadratic interpolation
            s = a*ya*yc/(yab*yac)+ b*ya*yc/(yab*ycb)-c*ya*yb/(yac*ycb);
          }
        else
          {
            // secant
            s = b -yb*(b-a)/(yb-ya);
          }
//
        if ( ( (s-3*(a+b)/4)*(s-b)>=0 ) or                           // condition 1
             ( mflag  and (std::abs(s-b)>=0.5*std::abs(b-c) ) ) or  // condition 2
             ( !mflag and (std::abs(s-b)>= 0.5*std::abs(c-d)) ) or // condition 3
             (  mflag and (std::abs(b-c) < tol) ) or               // condition 4
             ( !mflag and (std::abs(c-d) <tol) )                 // condition 5
            )
          {
            mflag=true;
            s = 0.5*(a+b); // back to bisection step
            }
        else
          mflag=false;
 //
        ys = f(s);
        d  = c;
        c  = b;
        yc = yb;
//
        if( ya*ys <0 )
          {
            b  = s;
            yb = ys;
          }
        else
          {
            a  = s;
            ya = ys;
          }
//
        if(std::abs(ya) < std::abs(yb))
          {
            std::swap(a,b  );
            std::swap(ya,yb);
          }
//
        }
    while (ys!=0. && std::abs(b-a)>tol && iter<maxIter);
    return {s,iter<maxIter};
}
} // end namespace

#endif /* EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_BASICZEROFUN_HPP_ */
