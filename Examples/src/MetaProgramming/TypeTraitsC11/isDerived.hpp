/*
 * isDerived.hpp
 *
 *  Created on: Nov 16, 2008
 *      Author: forma
 */

#ifndef ISDERIVED_HPP_
#define ISDERIVED_HPP_
#include <type_traits>
/*!
 * This is a first example of metaprogramming. Some features of the C++
 * template instantiation and static member initialisation are used to craft
 * a class able to check whether a class (D) is publicly derived by another
 * class (B).
 * C++11 has a type trait of that sort.
 */

//! This fails if D* is not convertible to B*
template <class B, class D>
using convertible_t = decltype(static_cast<B*>(static_cast<D*>(nullptr)));

template <class B,class D>
struct IsDerived
{
private:
  //! SFINAE if DD* is not convertible to BB* because of third template parameter
  template<class BB, class DD,class=convertible_t<BB,DD> >
  static std::true_type try_convert(BB&&, DD&&);
  //! Overloading. This is chosed if the previous template function is not viable
  static std::false_type try_convert(...);
public:
  //! type is true_type if D derives publicly from B
  using type=decltype(try_convert(std::declval<B>(),std::declval<D>()));
  static constexpr bool value=type::value;
                      
};



#endif /* ISDERIVED_HPP_ */
