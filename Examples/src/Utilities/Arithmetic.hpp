/**
 * @file Arithmetic.hpp
 * @brief Arithmetic concepts and type traits for numeric types.
 * @date Mar 13, 2023
 * @author forma
 */

#ifndef UTILITIES_ARITHMETIC_HPP_
#define UTILITIES_ARITHMETIC_HPP_
#include "is_complex.hpp"
#include <concepts>
namespace apsc
{
namespace TypeTraits
{
  /*!
   * Unfortunately the standard has not defined an arithmetic concept
   * Here Arithmetic means floating point or integral type
   */
  template <class T>
  concept Arithmetic = std::floating_point<T> || std::integral<T>;

  /*!
   * The ExtendedArithmetic concept adds also complex<T> with T arithmetic
   */
  template <class T>
  concept ExtendedArithmetic = Arithmetic<T> || ArithmeticComplex<T>;

} // namespace TypeTraits
} // namespace apsc

#endif /* UTILITIES_ARITHMETIC_HPP_ */
