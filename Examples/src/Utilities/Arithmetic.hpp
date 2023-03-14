/*
 * Arithmetic.hpp
 *
 *  Created on: Mar 13, 2023
 *      Author: forma
 */

#ifndef UTILITIES_ARITHMETIC_HPP_
#define UTILITIES_ARITHMETIC_HPP_
#include <concepts>
namespace apsc
{
  namespace TypeTraits
  {
    /*!
     * UNfortunately the standard has not defined an arithmetic concept
     */
    template<class T>
    concept Arithmetic=std::floating_point<T> || std::integral<T>;

  }
}




#endif /* UTILITIES_ARITHMETIC_HPP_ */
