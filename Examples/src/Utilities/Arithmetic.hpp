/*
 * Arithmetic.hpp
 *
 *  Created on: Mar 13, 2023
 *      Author: forma
 */

#ifndef UTILITIES_ARITHMETIC_HPP_
#define UTILITIES_ARITHMETIC_HPP_
#include <concepts>
#include "is_complex.hpp"
namespace apsc
{
  namespace TypeTraits
  {
    /*!
     * UNfortunately the standard has not defined an arithmetic concept
     * Here Arithmetic means floating point or integral type
     */
    template<class T>
    concept Arithmetic=std::floating_point<T> || std::integral<T>;

    /*!
     * The ExtendedArithmetic concept adds also complex<T> with T arithmetic
     */
    template<class T>
    concept ExtendedArithmetic=Arithmetic<T> || ArithmeticComplex<T>;

  }
}




#endif /* UTILITIES_ARITHMETIC_HPP_ */
