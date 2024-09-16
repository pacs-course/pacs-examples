#ifndef __HORNER_HPP__
#define __HORNER_HPP__
#include <functional>
#include <vector>
//! It evaluates a polynomial using standard rule.
/*!
  \f$ y=a_0+a_1x+\ldots+a_nx^n\f$

  @param a vector containing the coefficients from lowest to highest order.
  @param x evaluation point.
 */
double eval(std::vector<double> const &a, double const &x);

//! It evaluates a polynomial using horner rule
/*!
    \f$ y=(\ldots(a_n x+a_{n-1})x+a_{n-2})x\ldots +a_0   \f$

    @param a vector containing the coefficients from lowest to highest order.
    @param x evaluation point.
*/
double horner(std::vector<double> const &a, double const &x);

//! It evaluates a polynomial using horner rule. Version using c++20 ranges
/*!
    \f$ y=(\ldots(a_n x+a_{n-1})x+a_{n-2})x\ldots +a_0   \f$

    @param a vector containing the coefficients from lowest to highest order.
    @param x evaluation point.
*/
double horner_range(std::vector<double> const &a, double const &x);

// using a function wrapper to specify the policy
using polyEval =
  std::function<double(std::vector<double> const &, double const &)>;
//! Evaluates polynomial in a set of points.
/*!
  @param point   Vector of points to compute the polynomial.
  @param a       Polynomial coefficients.
  @result        A vector with the evaluated points
  @param method  Method to evaluate the polynomial
  @note if compiled with PARALLEXEC defined, it will use std::transform with
  parallel execution.
 */
std::vector<double> evaluatePoly(std::vector<double> const &points,
                                 std::vector<double> const &a, polyEval method);

#endif
