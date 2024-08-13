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

//*! A trait to compute the inner product of two arrays
/*!
 * The trait is used to compute the inner product of two arrays. The
 * trait is used to identify the type of the inner product, which is
 * T if T is a POD, and C if T is a complex::<C>. The trait is also
 * used to identify the operation to be performed on the elements of
 * the array, since the operation is different for complex than for
 * other types.
 * So this trait is also a policy since it can be used to change
 * the implementation for a particular class of object.
 */
template <typename T> struct Value_Trait
{
  //! The type
  using type = T;
  //! Product of two POD values
  //! I use contexpr to have a compile-time evaluation in case of contexpr arguments
  //!@note I have chosen to make the method static. I could hae instead made it 
  //! a const method. Having it static allows me to use it without an object.
  static constexpr 
  type
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
// This utility spares you the hideous typpename when exrtracting the type from the trait
template <typename T> 
using Value_Trait_t = typename Value_Trait<T>::type;
}// end nemespace




#endif /* TEMPLATES_TRAITS_INNERPRODUCTTRAITS_HPP_ */
