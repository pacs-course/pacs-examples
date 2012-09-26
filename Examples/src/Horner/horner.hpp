#ifndef __HORNER_HPP__
#define __HORNER_HPP__
#include <vector>
//! It evaluates a polynomial using standard rule.
/*!
  \f$ y=a_0+a_1x+\ldots+a_nx^n\f$

  @param a vector containing the coefficients from lowest to highest order.
  @param x evaluation point.
 */
double eval(std::vector<double> const & a, double const x);

//! It evaluates a polynomial using horner rule
/*!
    \f$ y=(\ldots(a_nx+a_{n-1})x+a_{n-2})x\ldots +a_0   \f$
    
    @param a vector containing the coefficients from lowest to highest order.
    @param x evaluation point.
*/
double horner(std::vector<double> const & a, double const x);

//This only to show the use of pointers to function.
//! A pointer to function.
typedef double (*polyEval)(std::vector<double> const &, double const);

//! Evaluates polynomial in a set of points.
/*!
  @param point   Set of points to compute the polynomial.
  @param a       Polynomial coefficients.
  @param result  As the name says.
  @param method  Method to evaluate the polynomial
 */
void evaluatePoly(std::vector<double> const & points,
		  std::vector<double> const & a,
		  std::vector<double>  & result, polyEval method);
#endif
