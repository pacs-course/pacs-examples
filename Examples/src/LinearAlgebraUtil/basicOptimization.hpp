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
#include <tuple>
#include <numbers> // C++20 stuff to get golden ration
/*
 * In this version (march 2023) I use concepts to constrain the
 * template parameter indicating the function. Function must be
 * double (double) or double (double const &)
 */
#include "functionConcepts.hpp"// from Utility

namespace apsc
{
/*!
 *  Golden search method to find the extrema of a function. The function must be
 *  double (double) or double (double const &) and must be unimodal in the interval.
 *
 * @tparam Function The type of a callable object double (double)
 * @param f The callable object double (double)
 * @param a the first end of an interval bracketing the extremum
 * @param b the second end of an interval bracketing the extremum
 * @param tol The desired tolerance. It is guaranteed that the found extremum is
 * within the tolerance
 * @param maxIter Max number of iterations. Safeguard against non-convergence
 * @return The found extremum and a status (=false max iterations reached)
 */
template <TypeTraits::ScalarFunction Function>
std::tuple<double, bool>
golden_search(Function const &f, double a, double b, double tol = 1.e-5,
              unsigned maxIter = 100)
{
  constexpr double phi = std::numbers::phi_v<double>;//(std::sqrt(5.) + 1) / 2;
  unsigned         iter{0u};
  while(std::abs(b - a) > tol && iter < maxIter)
    {
      auto c = b - (b - a) / phi;
      auto d = a + (b - a) / phi;
      ++iter;
      if(f(c) < f(d))
        b = d;
      else
        a = c;
    }
  return {0.5 * (a + b), iter < maxIter};
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
bracketIntervalMinumum(Function const &f, double x1, double h = 0.01,
                       unsigned int maxIter = 100)
{
  constexpr double expandFactor = 1.5;
  int              status = 0;
  // First test
  auto y1 = f(x1);
  // Check monotonicity
  if(f(x1 - h) < y1 and f(x1 + h) > y1)
    h = -h;                                  // change direction
  else if(f(x1 - h) > y1 and f(x1 + h) > y1) // I have found the bracket
    return {x1 - h, x1 + h, 0};
  else if(y1 > f(x1 - h) and y1 > f(x1 - h)) // bad initial point! Cannot start!
    return {0., 0., 1};
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
    }
  while(!isOk and iter < maxIter);
  if(iter >= maxIter)
    status = 2;
  return std::make_tuple(x1, x2, status);
}

} // namespace apsc

#endif /* EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_BASICOPTIMIZATION_HPP_ */
