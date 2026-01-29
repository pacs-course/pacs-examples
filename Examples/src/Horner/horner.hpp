#ifndef __HORNER_HPP__
#define __HORNER_HPP__
#include <functional>
#include <vector>
//! It evaluates a polynomial using the standard (naive) rule, i.e., by directly computing each term.
/*!
  \f$ y=a_0+a_1x+\ldots+a_nx^n\f$

  @param a vector containing the coefficients from lowest to highest order.
  @param x evaluation point.
 */
/*!
  \brief Evaluates a polynomial at a given point using the standard rule.
//! It evaluates a polynomial using Horner's method
  @param a Vector containing the coefficients from lowest to highest order.
  @param x Evaluation point.
  @return The value of the polynomial at point x.
*/
double eval(std::vector<double> const &a, double const &x);

//! It evaluates a polynomial using Horner's method
/*!
    \f$ y=(\ldots(a_n x+a_{n-1})x+a_{n-2})x\ldots +a_0   \f$

    @param a vector containing the coefficients from lowest to highest order.
    @param x evaluation point.
*/
double horner(std::vector<double> const &a, double const &x);

//! It evaluates a polynomial using Horner's method. Version using C++20 ranges
/*!
    \f$ y=(\ldots(a_n x+a_{n-1})x+a_{n-2})x\ldots +a_0   \f$

    @param a vector containing the coefficients from lowest to highest order.
    @param x evaluation point.
*/
double horner_range(std::vector<double> const &a, double const &x);

// Using a function wrapper to specify the evaluation policy (i.e., which method to use for polynomial evaluation, such as naive evaluation, Horner's method, etc.)
using polyEval =
  std::function<double(std::vector<double> const &, double const &)>;
//! Evaluates polynomial in a set of points.
/*!
  @param point   Vector of points to compute the polynomial.
  @param a       Polynomial coefficients.
  @result        A vector with the evaluated points
  @param method  Function object (policy) to evaluate the polynomial, e.g., naive evaluation or Horner's method.
  @note if compiled with PARALLEXEC defined, it will use std::transform with
  parallel execution.
 */
std::vector<double> evaluatePoly(std::vector<double> const &points,
                                 std::vector<double> const &a, polyEval method);

#endif
