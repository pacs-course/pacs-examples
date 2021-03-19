#ifndef HORNER_HPP
#define HORNER_HPP

#include <vector>

//! It evaluates a polynomial using standard rule.
/*!
  \f$ y=a_0+a_1x+\ldots+a_nx^n\f$

  @param a vector containing the coefficients from lowest to highest order.
  @param x evaluation point.
 */
double
eval(const std::vector<double> &a, const double &x);

//! It evaluates a polynomial using horner rule
/*!
    \f$ y=(\ldots(a_nx+a_{n-1})x+a_{n-2})x\ldots +a_0   \f$

    @param a vector containing the coefficients from lowest to highest order.
    @param x evaluation point.
*/
double
eval_horner(const std::vector<double> &a, const double &x);

// This only to show the use of pointers to function.
//! A pointer to function.
//
// typedef double (*eval_method)(const std::vector<double> &, const
// double); In C++11 it is preferable to do
using eval_method = double (*)(const std::vector<double> &,
                               const double &);

//! Evaluates polynomial in a set of points.
/*!
  @param points  Vector of points to compute the polynomial.
  @param a       Polynomial coefficients.
  @result        A vector with the evaluated points
  @param method  Method to evaluate the polynomial
 */
std::vector<double>
evaluate_poly(const std::vector<double> &points,
              const std::vector<double> &a,
              eval_method                method);

#endif /* HORNER_HPP */
