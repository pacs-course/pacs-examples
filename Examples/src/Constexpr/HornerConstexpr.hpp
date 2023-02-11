/*
 * HornerConstexpr.hpp
 *
 *  Created on: May 22, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_CONSTEXPR_HORNERCONSTEXPR_HPP_
#define EXAMPLES_SRC_CONSTEXPR_HORNERCONSTEXPR_HPP_
#include <type_traits>
/*!
 * A generic implementation of Horner rule for computing polynomials
 *
 * This implementation allows to treat both real and complex polynomials,
 * moreover, if the arguments are constant expressions the compiler may compute
 * the result statically
 *
 * @tparam Coeff The type of the container with the coefficients: either
 * std::array or std::vector
 * @tparam Value The type of the point where the polynomial must be computed
 * @param coeff the coefficients
 * @param x The point where polynomial is computed
 * @return The evaluation of the polynomial. The type is the common type between
 * coefficients and point x
 * @pre The coefficients must store at least one element.
 */
template <class Coeff, class Value>
constexpr auto
HornerConstexpr(Coeff const &coeff, Value const &x)
{
  // The type of u is the common type among the coefficient and the value x
  std::common_type_t<decltype(coeff.back()), Value> u =
    coeff.back(); // back is a constexpr function for arrays
                  // and size() is a constexpr function for arrays
  for(int i = coeff.size() - 2; i >= 0; --i)
    u = u * x + coeff[i]; // [] operator is constexpr for arrays
  return u;
}

#endif /* EXAMPLES_SRC_CONSTEXPR_HORNERCONSTEXPR_HPP_ */
