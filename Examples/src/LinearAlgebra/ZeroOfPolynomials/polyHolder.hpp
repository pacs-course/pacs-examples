/*
 * polyRoot.hpp
 *
 *  Created on: Sep 23, 2021
 *      Author: forma
 */
/*!
 * @brief Utilities for polynomial evaluation, differentiation and zero finding
 *
 * The core idea is Newton-Horner iteration: synthetic division is used to
 * evaluate both p(x) and the coefficients of the deflated quotient q(x) in a
 * single pass. Since p(x) = (x - alpha) q(x) + p(alpha), the derivative can be
 * recovered as p'(alpha) = q(alpha). This makes Newton's method particularly
 * convenient for polynomials.
 */
#ifndef EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_POLYHOLDER_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_POLYHOLDER_HPP_
#include <algorithm>
#include <complex>
#include <concepts>
#include <limits>
#include <ranges>
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
 * @brief Stores a polynomial and the temporary quotient produced by synthetic
 * division
 *
 * It works internally with complex numbers so that it can also compute complex
 * roots, even if the polynomial has been given by real coefficients.
 *
 * The class stores two coefficient vectors:
 * - `pCoeff`: the current polynomial
 * - `qCoeff`: the quotient obtained by dividing `pCoeff` by `(x - alpha)` at a
 *   chosen evaluation point `alpha`
 *
 * Synthetic division therefore provides both the residual `p(alpha)` and the
 * data needed for the Newton-Horner step. The same mechanism is reused to
 * compute derivatives.
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
   * @brief Computes derivatives up to a given order at a point
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
   * The computation starts from one synthetic division on the original
   * polynomial. For a polynomial
   * `p(z) = (z - x) q(z) + p(x)`,
   * one has `p'(x) = q(x)`. Repeating the same idea on `q` yields higher
   * derivatives, up to the appropriate factorial factor.
   *
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
   * @brief Replaces the current polynomial with the last quotient polynomial
   *
   * This helper is used only in the recursive derivative construction. Despite
   * its name, it is not a symmetric swap: `pCoeff` is overwritten with the
   * contents of `qCoeff`.
   */
  void
  swap()
  {
    pCoeff = std::move(qCoeff);
  }

  /*!
   * @brief Performs synthetic division by `(z - x)`
   *
   * If
   * `p(z) = a_0 + a_1 z + ... + a_n z^n`,
   * this routine builds the quotient coefficients of
   * `p(z) = (z - x) q(z) + p(x)`.
   *
   * The returned value is the remainder `p(x)`, while `qCoeff` stores the
   * quotient polynomial `q`. In Newton-Horner, evaluating `q` again at `x`
   * gives `p'(x)`.
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
 * @brief Finds polynomial roots with a Newton-Horner iteration plus deflation
 *
 * @tparam Coefficients The container for the coefficients of the polynomial
 * Typically `std::vector<std::complex<double>>`
 * @tparam Coefficient The type of the polynomial coefficient (typically
 * `std::complex<double>`)
 * The algorithm repeatedly:
 * 1. applies Newton's method to the current polynomial,
 * 2. accepts one root when both the residual and the Newton step are small,
 * 3. deflates the polynomial by dividing by `(x - root)`,
 * 4. restarts from the conjugate root as a cheap heuristic for real
 *    polynomials.
 *
 * The coefficients are first normalized by removing trailing entries that are
 * numerically zero. This avoids treating a lower-degree polynomial as a higher
 * one because of redundant trailing coefficients.
 *
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
  auto normalizedCoeff = polyHolder.pCoefficients();
  if(!normalizedCoeff.empty())
    {
      // Use a scale-dependent threshold so that tiny trailing coefficients do
      // not artificially increase the degree.
      auto const scale = std::ranges::max(
        normalizedCoeff | std::views::transform([](auto const &c) {
          return std::abs(c);
        }));
      auto const coeffTol =
        std::numeric_limits<double>::epsilon() * (1.0 + scale);
      while(!normalizedCoeff.empty() &&
            std::abs(normalizedCoeff.back()) <= coeffTol)
        {
          normalizedCoeff.pop_back();
        }
      polyHolder.setCoeff(normalizedCoeff);
    }

  PolyHolder::Coefficient x{x0};
  auto const              coeff = polyHolder.pCoefficients();
  auto const              numCoeff = coeff.size();
  std::vector<std::complex<double>> roots;
  std::vector<double>               residual;
  if(numCoeff == 0u)
    {
      // The zero polynomial does not have a finite set of isolated roots, so
      // the current interface reports failure.
      return {roots, residual, false};
    }
  if(numCoeff == 1u)
    {
      // A nonzero constant polynomial has no finite roots.
      return {roots, residual, true};
    }
  auto const degree = static_cast<unsigned int>(numCoeff - 1u);
  if(degree == 1u)
    {
      // For degree one, solve explicitly instead of entering Newton iteration.
      roots.emplace_back(-coeff[0] / coeff[1]);
      residual.emplace_back(0.);
      return {roots, residual, true};
    }
  numRoots = std::min(numRoots, degree);
  roots.reserve(numRoots);
  residual.reserve(numRoots);
  bool         status{true};
  for(auto i = 0u; i < numRoots; ++i)
    {
      bool   good = false;
      double res = std::numeric_limits<double>::infinity();
      for(unsigned int iter = 0u; iter < maxIter; ++iter)
        {
          auto values = polyHolder.derivatives(x, 1u);
          auto const fx = values[0];
          auto const dfx = values.size() > 1u ? values[1] : PolyHolder::Coefficient{};
          auto const dfxNorm = std::abs(dfx);
          if(dfxNorm <= std::numeric_limits<double>::epsilon())
            {
              if(std::abs(fx) < tolr)
                {
                  // We are already close enough to a root, even if the local
                  // derivative is numerically singular.
                  res = std::abs(fx);
                  good = true;
                  break;
                }
              // Avoid the meaningless step -fx/0 by perturbing the iterate and
              // trying again. This is only a safeguard, not a robust
              // globalization strategy.
              auto const shift = std::max(1.e-3 * tole, 1.e-12);
              x += PolyHolder::Coefficient{shift, shift};
              continue;
            }

          // Newton-Horner step on the current deflated polynomial.
          auto const delta = -fx / dfx;
          auto const xNext = x + delta;
          // Test convergence at the accepted iterate, not at the previous one.
          res = std::abs(polyEval(polyHolder.pCoefficients(), xNext));
          x = xNext;
          if((res < tolr) && (std::abs(delta) < tole))
            {
              good = true;
              break;
            }
        }

      roots.emplace_back(x);
      residual.emplace_back(res);
      status = status && good;
      if(!good)
        {
          break;
        }

      // Recompute the quotient polynomial at the accepted root before
      // deflation. The previous `qCoeff` was built at the last trial iterate,
      // which may differ from the accepted one.
      polyHolder.derivatives(x, 1u);
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
