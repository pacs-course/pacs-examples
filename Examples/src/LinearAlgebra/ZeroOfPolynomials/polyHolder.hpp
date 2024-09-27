/*
 * polyRoot.hpp
 *
 *  Created on: Sep 23, 2021
 *      Author: forma
 */
/*!
 * @brief An utility for the zeros of polynomials
 *
 */
#ifndef EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_POLYHOLDER_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_POLYHOLDER_HPP_
#include <algorithm>
#include <complex>
#include <concepts>
#include <limits>
#include <tuple>
#include <utility>
#include <vector>
namespace apsc
{
/*!
  * @brief Helper function. Computes the polynomial given the coefficients
  *
  * It is just a different version of Holder algorithm already seen in other
  examples.
  * coefficient are stored in a container, ordered from lowest to highest
  monomial degree.
  * The function is templated on the container type and on the coefficient type.
  * The coefficient type must be convertible to a complex<double> number.

  * @pre Coefficients is a container of Coefficient.
  * @tparam Coefficients The container of coefficients (must be a sequential
  container)
  * @tparam Coefficient The type of a polynomial coefficient
  * @param pCoeff The coefficients
  * @param x The value where to compute the polynomual
  * @return The polynomial at x
  * @note added a requires clause to check the concept of the input
  */
template <class Coefficients, class Coefficient>
  requires requires(Coefficients cs, Coefficient c) {
             Coefficient{0}; // constructible with 0.
             cs[0];          // has addressing operator
             cs.rbegin();    // has method rbegin
             cs.rend();      // has method rend
           }
auto
polyEval(const Coefficients &pCoeff, const Coefficient &x)
{
  if(pCoeff.size() == 1u)
    return pCoeff[0];
  else if(pCoeff.size() == 0u)
    return Coefficient{0.};
  auto startp = pCoeff.rbegin();
  auto value = *startp;
  ++startp;
  while(startp != pCoeff.rend())
    {
      value = value * x + *(startp++);
    }
  return value;
}

/*!
 * @brief A class to support root finding for algebraic polynomials
 *
 * It works internally with complex numbers to be able to compute also complex
 * roots, even if the polynomial has been given by real coefficients.
 *
 * It also provide a method to compute derivatives of polynomials at a point
 * using synthetic division. It is provided here since it is a simple extension
 * of what needed for the root finding. An exception to the one responsibility
 * principle.
 *
 * Alterrnatively, one may build a class to encapsulate the synthetic division
 * an  keep root finding and derivatives separate. This would be more in line
 * with the one responsibility principle.
 *
 * The algorithm should always work for polynomial with real coefficients. For
 * polynomyal on the complex field however, the situation is more complex. Look
 * at specialised literature on the subject.
 *
 */
class PolyHolder
{
public:
  //! The coefficient (and values) type
  using Coefficient = std::complex<double>;
  //! The container for the coefficients of the polynomials from lowest to
  //! highest
  using Coefficients = std::vector<Coefficient>;
  /*!
   * @brief Constuctor
   *
   * @tparam T anything convertible to a vector of complex numbers
   * @param coeff coefficients for the polynomial
   */
  template <class T>
    requires std::convertible_to<T, Coefficients>
  PolyHolder(T &&coeff) : pCoeff(std::forward<T>(coeff)){};
  /*!
   * @brief Constuctor
   * Specialization if the coefficients are real. Internally I use complex.
   * @param coeff Coefficients of the polynomial (lowest to highest)
   */
  PolyHolder(std::vector<double> const &coeff)
  {
    pCoeff.reserve(coeff.size());
    for(auto const &x : coeff)
      {
        pCoeff.emplace_back(x, 0.);
      }
  }
  /*!
   * @brief Default Constructor
   */
  PolyHolder() = default;

  /*!
   * @brief Set new polynomial coefficients
   *
   * @tparam T anything convertible to vector of complex
   * @param coeff Coefficients
   */
  template <class T>
    requires std::convertible_to<T, Coefficients>
  void
  setCoeff(T &&coeff)
  {
    pCoeff = std::forward<T>(coeff);
    qCoeff.clear();
  }
  /*!
   * @brief Sets coefficients
   * Version for real coefficients.
   * I need to convert them to complex
   * @param coeff
   */
  void
  setCoeff(std::vector<double> const &coeff)
  {
    pCoeff.clear();
    pCoeff.shrink_to_fit();
    pCoeff.reserve(coeff.size());
    for(auto const &x : coeff)
      {
        pCoeff.emplace_back(x, 0.);
      }
    qCoeff.clear();
  }

  /*!
   * @brief Computes derivatives up to given order
   *
   *  All derivatives at given point and up to given order are computed and
   * stored in the returned vector of complex numbers. Derivative of order 0 is
   * the value at x.
   *
   *  For Newton-Horner algorithm for the zeroes of a polynomial only the first
   * order derivative is needed. Nevertheless, since it is not a big problem
   * extending to the case of a generic derivative I have implemented the
   * general case. First derivative is the default.
   *
   * @param x The point where derivatives are computed
   * @param order The order of derivative to be computed (defaulted to 1)
   * @return A vector with all the derivative (at 0 you have the 0th derivative
   * = the value at x)
   */
  std::vector<std::complex<double>>
  derivatives(Coefficient const &x, unsigned int order = 1) const
  {
    std::vector<std::complex<double>> values;
    const unsigned int                maxorder = pCoeff.size();
    if(order == 0u)
      {
        values.emplace_back(polyEval(this->pCoeff, x));
      }
    else
      {
        values.emplace_back(this->sintDivision(x));
        values.emplace_back(polyEval(this->qCoeff, x));
        // this is the case of derivatives higher than one. I apply synthetic
        // division recursively
        if(order > 1)
          {
            PolyHolder   tmp(this->qCoeff);
            unsigned int coeff{1u};
            for(unsigned i = 2; i <= std::min(order, maxorder); ++i)
              {
                tmp.sintDivision(x);
                coeff *= i;
                auto d = static_cast<double>(coeff) * polyEval(tmp.qCoeff, x);
                values.emplace_back(d);
                tmp.swap();
              }
            for(unsigned i = std::min(order, maxorder) + 1; i <= order; ++i)
              {
                values.emplace_back(0., 0.);
              }
          }
      }
    return values;
  }
  /*!
   * @return Polynomial coefficients (readOnly);
   */
  auto
  pCoefficients() const
  {
    return pCoeff;
  }
  /*!
   * @return q-Polynomial coefficients (readOnly);
   */
  auto
  qCoefficients() const
  {
    return qCoeff;
  }
  /*!
   * @brief The  coefficients of the nth derivative
   *
   * If n=0 it is a stupid way of getting the polynomial coefficient. Indeed for
   * n=0 the derivative is the polynomial itself. This is a different algorithm
   * than the one presented in the folder src/Polynomials. It is a not recursive
   * implementation, that however needs the computation of a factorial
   * coefficient, which is in the protected section since it is not of general
   * use.
   *
   * @pre Polynomial coefficients must be set
   * @param n The order of the derivative.
   * @return The coefficients of the n-th order derivative of the stored
   * polynomial
   */
  Coefficients
  derivativeCoeff(unsigned int n) const
  {
    auto const numCoeff = pCoeff.size();
    if(n >= numCoeff)
      return Coefficients{{0., 0.}};
    auto         numDerCoeff = numCoeff - n;
    Coefficients derCoeff(numDerCoeff);
    for(std::size_t p = n; p < numCoeff; ++p)
      {
        derCoeff[p - n] = static_cast<double>(factor(p, n)) * pCoeff[p];
      }
    return derCoeff;
  }

protected:
  /*!
   * @brief Exchange p and q coefficients. Used for computing derivatives
   * Is not a real swap. The new coefficients are the old q's and the old q is
   * cleared Not for general use.
   */
  void
  swap()
  {
    pCoeff = std::move(qCoeff);
  }

  /*!
   * @brief Synthetic division
   *
   * It computes the value of the polynomial at a point and sets qCoeff for the
   * calculation of the derivatives.
   *
   * Used internally. Not of general use.
   *
   * @pre pCoeff should have been set
   * @param x The point
   * @return The value of the polynomial at the point
   */
  Coefficient
  sintDivision(Coefficient const &x) const
  {
    qCoeff.clear();
    // special cases
    if(pCoeff.size() == 1u)
      return pCoeff[0];
    else if(pCoeff.size() == 0u)
      return Coefficient{0, 0};
    qCoeff.resize(pCoeff.size() - 1u);
    // points to the end of p
    auto startp = pCoeff.rbegin();
    // point to the end of q
    auto startq = qCoeff.rbegin();
    auto endq = qCoeff.rend();
    auto prevq = startq;
    // q_{n-1} = p_n
    *(startq++) = *(startp++);
    while(startq != endq)
      {
        // q_k = q_{k+1}* x + p_k
        *startq = *prevq * x + *(startp++);
        prevq = startq++;
      }
    return (*prevq) * x + *startp;
  }
  /*!
   * @brief  Computes p(p-1)(p-2)...(p-n+1)= n! (p|n)
   *
   * It is a helper function, I put it in the protected section since it is not
   * of general use
   *
   * Returns 1 if p<=n
   * @param p
   * @param n
   * @return p(p-1)(p-2)...(p-n+1)
   */
  static constexpr unsigned int
  factor(unsigned int p, unsigned int n = 0)
  {
    return (n == 0 || p < n) ? 1u : factor(p - 1u, n - 1) * p;
  }
  Coefficients pCoeff; //!< The coefficients of the polynomial
  /*!
   * The coefficients of the associated polynomial.
   * Must be mutable since they may change also on "morally constant" objects.
   */
  mutable Coefficients qCoeff;
};
/*!
 * @brief Finds roots of a polynomial
 *
 * @tparam Coefficients The container for the coefficients of the polynomial
 * Typically a std::vector<complex>
 * @tparam Coefficient The type of the polynomial coefficient (typycally
 * std::complex)
 * @param polyCoefficients The coefficients of the polynomial, in increasing
 * order
 * @param numRoots The number of roots sought (max=the degree of polynomial)
 * @param x0 The initial point for the search.  If you want to find all roots
 * the complex part should be !=0
 * @param tole Tolerance on iteration steps (default 1e-6)
 * @param tolr Tolerance on residual (|p(x)|) (default 1e-6)
 * @param maxIter The maximum number of iterations (default=200)
 * @return A tuple with the vector of found zeros, the vector of residuals and a
 * status (true=ok, false=failure).
 */

template <class Coefficients, class Coefficient>
std::tuple<std::vector<std::complex<double>>, std::vector<double>, bool>
polyRoots(Coefficients &&polyCoefficients, unsigned int numRoots,
          Coefficient const &x0, double tole = 1.e-6, double tolr = 1.e-6,
          unsigned int maxIter = 200)
{
  PolyHolder polyHolder(std::forward<Coefficients>(polyCoefficients));
  PolyHolder::Coefficient           x{x0};
  unsigned int const                degree = polyHolder.pCoefficients().size();
  std::vector<std::complex<double>> roots;
  std::vector<double>               residual;
  if(degree == 0u)
    {
      return {roots, residual, false};
    }
  numRoots = std::min(numRoots, degree - 1u);
  roots.reserve(numRoots);
  residual.reserve(numRoots);
  unsigned int iter{0u};
  bool         status{true};
  for(auto i = 0u; i < numRoots; ++i)
    {
      bool good = false;
      iter = 0u;
      double res;
      while(!good)
        {
          auto values = polyHolder.derivatives(x, 1u);
          // to avoid division by zero. If the denominator is very small
          // I do one step of a basic fixed point scheme: x_{n+1}== x_n-p(x)
          auto d = std::abs(values[1]) == 0.0 ? std::complex<double>{1., 0.}
                                              : values[1];
          auto delta = -values[0] / d;
          x += delta;
          // To make the loop simpler I am testing the residual at
          // the previous iteration, not that in x.
          res = std::abs(values[0]);
          good =
            ((res < tolr) && (std::abs(delta) < tole)) || (iter++ >= maxIter);
        }
      // store the root
      roots.emplace_back(x);
      // store the residual
      residual.emplace_back(res);
      // check status (false if at least one of the roots search failed).
      status = status && (iter <= maxIter);
      // Store deflated polynomial
      polyHolder.setCoeff(polyHolder.qCoefficients());
      // Start next search from conjugate (to speed up computation if
      // coefficients are real)
      x = std::conj(x);
    }
  return {roots, residual, status};
}

} // namespace apsc

#endif /* EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_POLYHOLDER_HPP_ */
