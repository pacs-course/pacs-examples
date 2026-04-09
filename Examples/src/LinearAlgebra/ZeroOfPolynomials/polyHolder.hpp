/*
 * polyRoot.hpp
 *
 *  Created on: Sep 23, 2021
 *      Author: forma
 */
/*!
 * @brief A utility for finding polynomial zeros
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
  * @brief Helper function. Evaluates a polynomial from its coefficients.
  *
  * It is simply a different version of Horner's algorithm, already seen in
  * other examples.
  * Coefficients are stored in a container, ordered from the lowest to the
  * highest monomial degree.
  * The function is templated on the container type and on the coefficient type.
  * The coefficient type must be convertible to a `std::complex<double>`.

  * @pre Coefficients is a container of Coefficient.
  * @tparam Coefficients The container of coefficients (must be a sequential
  * container)
  * @tparam Coefficient The type of a polynomial coefficient
  * @param pCoeff The coefficients
  * @param x The value at which the polynomial is evaluated
  * @return The polynomial value at x
  * @note A requires clause checks the expected interface of the input.
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
 * @brief A class supporting root finding for algebraic polynomials
 *
 * It works internally with complex numbers so that it can also compute complex
 * roots, even if the polynomial has been given by real coefficients.
 *
 * It also provides a method to compute polynomial derivatives at a point
 * using synthetic division. It is provided here since it is a simple extension
 * of what is needed for root finding. This is an exception to the
 * single-responsibility principle.
 *
 * Alternatively, one could build a separate class to encapsulate synthetic
 * division and keep root finding and derivative computation separate. That
 * would be more in line with the single-responsibility principle.
 *
 * The algorithm should always work for polynomials with real coefficients. For
 * polynomials over the complex field, however, the situation is more delicate.
 * See the specialized literature on the subject.
 *
 */
class PolyHolder
{
public:
  //! The coefficient (and values) type
  using Coefficient = std::complex<double>;
  //! The container for the polynomial coefficients, from lowest to highest
  using Coefficients = std::vector<Coefficient>;
  /*!
   * @brief Constructor
   *
   * @tparam T Any type convertible to a vector of complex numbers
   * @param coeff Coefficients of the polynomial, ordered from the lowest to
   * the highest monomial degree. The container should provide the coefficients
   * in increasing order of degree.
   */
  template <class T>
    requires std::convertible_to<T, Coefficients>
  PolyHolder(T &&coeff) : pCoeff(std::forward<T>(coeff)){};
  /*!
   * @brief Constructor
   * Specialized version for real coefficients. Internally, coefficients are
   * stored as complex numbers.
   * @param coeff Coefficients of the polynomial (from lowest to highest degree)
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
   * @brief Sets new polynomial coefficients
   *
   * @tparam T Any type convertible to a vector of complex numbers
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
   * @brief Sets the coefficients
   * Version for real coefficients.
   * The coefficients are converted to complex values internally.
   * @param coeff The real coefficients
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
   * @brief Computes derivatives up to a given order
   *
   * All derivatives at a given point and up to a given order are computed and
   * stored in the returned vector of complex numbers. Derivative of order 0 is
   * the value at x.
   *
   * For the Newton-Horner algorithm for the zeros of a polynomial, only the
   * first derivative is needed. Nevertheless, since extending the method to
   * generic derivatives is straightforward, I implemented the general case.
   * The first derivative is the default.
   *
   * @param x The point where the derivatives are computed
   * @param order The derivative order to be computed (default is 1)
   * @return A vector containing all derivatives. Entry 0 is the 0-th
   * derivative, that is, the value at x.
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
        // This is the case of derivatives of order higher than one.
        // I apply synthetic division recursively.
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
   * @return Polynomial coefficients (read-only).
   */
  auto
  pCoefficients() const
  {
    return pCoeff;
  }
  /*!
   * @return The coefficients of the associated q-polynomial (read-only).
   */
  auto
  qCoefficients() const
  {
    return qCoeff;
  }
  /*!
   * @brief Returns the coefficients of the n-th derivative
   *
   * If `n = 0`, this is simply an indirect way of retrieving the polynomial
   * coefficients, because the 0-th derivative is the polynomial itself.
   * This is a different algorithm from the one presented in the folder
   * `src/Polynomials`. It is a non-recursive implementation, but it requires
   * the computation of a factorial-like coefficient, implemented in the
   * protected section because it is not of general use.
   *
   * @pre Polynomial coefficients must be set
   * @param n The order of the derivative.
   * @return The coefficients of the n-th derivative of the stored
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
   * @brief Exchanges p and q coefficients. Used for computing derivatives.
   * This is not a true swap. The new coefficients become the old q-values,
   * and q is then cleared. Not intended for general use.
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
   * @pre pCoeff must have been set
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
   * @brief Computes p(p-1)(p-2)...(p-n+1) = n! * binomial(p, n)
   *
   * This is a helper function. It is placed in the protected section because
   * it is not of general use.
   *
   * Returns 1 if p <= n.
   * @param p The upper index
   * @param n The order
   * @return p(p-1)(p-2)...(p-n+1)
   */
  static constexpr unsigned int
  factor(unsigned int p, unsigned int n = 0)
  {
    if(n == 0 || p < n)
      return 1u;
    else
      return factor(p - 1u, n - 1) * p;
  }
  Coefficients pCoeff; //!< The coefficients of the polynomial
  /*!
   * The coefficients of the associated polynomial.
   * Must be mutable since they may change also on "morally constant" objects.
   */
  mutable Coefficients qCoeff;
};
/*!
 * @brief Finds the roots of a polynomial
 *
 * @tparam Coefficients The container for the coefficients of the polynomial
 * Typically `std::vector<std::complex<double>>`
 * @tparam Coefficient The type of the polynomial coefficient (typically
 * `std::complex<double>`)
 * @param polyCoefficients The coefficients of the polynomial, in increasing
 * order
 * @param numRoots The number of roots sought (max = the polynomial degree)
 * @param x0 The initial point for the search. If you want to find all roots,
 * the imaginary part should be different from zero.
 * @param tole Tolerance on the iteration step (default 1e-6)
 * @param tolr Tolerance on the residual `|p(x)|` (default 1e-6)
 * @param maxIter The maximum number of iterations (default = 200)
 * @return A tuple with the vector of found zeros, the vector of residuals and a
 * status (`true` = success, `false` = failure).
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
      // zero polynomial, no roots
      numRoots = 0u;
      return {roots, residual, false};
    }
  if(degree == 1u)
    {
      // linear polynomial, one root
      roots.emplace_back(-polyHolder.pCoefficients()[0] /
                         polyHolder.pCoefficients()[1]);
      residual.emplace_back(0.);
      numRoots = 1u;
      return {roots, residual, true};
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
          std::complex<double> d;
          if(values.size() > 1)
            d = std::abs(values[1]) == 0.0 ? std::complex<double>{1., 0.}
                                           : values[1];
          else
            d = std::complex<double>{
              1., 0.}; // fallback for constant/degenerate polynomial

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
      //@todo if coefficients are complex this may not be the best choice
      // maybe random perturbation?
      x = std::conj(x);
    }
  return {roots, residual, status};
}

} // namespace apsc

#endif /* EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_POLYHOLDER_HPP_ */
