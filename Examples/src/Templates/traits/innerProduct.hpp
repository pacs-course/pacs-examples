/*
 * innerProduct.hpp
 *
 *  Created on: Mar 21, 2023
 *      Author: forma
 */

#ifndef TEMPLATES_TRAITS_INNERPRODUCT_HPP_
#define TEMPLATES_TRAITS_INNERPRODUCT_HPP_
#include "innerProductTraits.hpp"
namespace apsc
{
  //! A function using traits. It computes the inner product.
  /*!
   * We need a trait to identify the correct return type, which should
   * be a T if T is a POD, and C if T is a complex::<C>. We need a
   * policy (stored in the same trait) to operate on each element of the
   * array, since the operation is different for complex than for other
   * types.
   *
   * @param x an array of values
   * @param y an array of values
   * @return the Euclidean inner product (x,y)
   */
  template <typename T, std::size_t N>
  constexpr
  auto
   innerProduct(const std::array<T, N> &x, const std::array<T, N> &y)
  {
    // Here I use the trait!
    //using valueType = typename Value_Trait<T>::type; //befor c++17
    using valueType = Value_Trait_t<T>; //since c++17
    valueType res{0}; // 0 is convertible to any POD
    for(std::size_t i = 0u; i < N; ++i)
      {
        // Here I use the trait!
        res += Value_Trait<T>::prod(x[i], y[i]);
      }
    return res;
  }

}



#endif /* TEMPLATES_TRAITS_INNERPRODUCT_HPP_ */
