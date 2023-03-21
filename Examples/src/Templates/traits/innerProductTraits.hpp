/*
 * innerProductTraits.hpp
 *
 *  Created on: Mar 21, 2023
 *      Author: forma
 */

#ifndef TEMPLATES_TRAITS_INNERPRODUCTTRAITS_HPP_
#define TEMPLATES_TRAITS_INNERPRODUCTTRAITS_HPP_
#include <array>
#include <cmath>
#include <complex>
namespace apsc{

//! The primary template
template <typename T> struct Value_Trait
{
  //! The type
  using type = T;
  //! Inner product. It works for all POD
  static type
  prod(T const &x, T const &y)
  {
    return x * y;
  }
};

//! Specialization for complex
template <class T> struct Value_Trait<std::complex<T> >
{
  //! The type of the complex field
  using type = T;
  //! The redefinition of the product
  static type
  prod(std::complex<T> const &x, std::complex<T> const &y)
  {
    return x.imag() * y.imag() + x.real() * y.real();
  }
};
}// end nemespace




#endif /* TEMPLATES_TRAITS_INNERPRODUCTTRAITS_HPP_ */
