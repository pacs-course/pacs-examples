/*
 * isDerived.hpp
 *
 *  Created on: Nov 16, 2008
 *      Author: forma
 */

#ifndef ISDERIVED_HPP_
#define ISDERIVED_HPP_
/*!This is an first example of metaprogramming.
 * This is an first example of metaprogramming. Some features of the C++
 * template instantiation and static member initialisation are used to craft
 * a class able to check whether a class (D) is publicly derived by another
 * class (B).
 * Note that C++11 has introcuced a trait called is_base_of<B,D> that does mainly
 * the same thing!
 */

//! Class that incorporates a bool
template <bool v>
struct bool_type
{
  //gives the enclodes value
  constexpr static bool value=v;
  //converts to a bool using enclosed value
  constexpr operator bool() const {return v;}
};

//! This traits are defined by C++11 in the header <type_traits>
/*!
  I redefine them here just to show how they work. In C++11 are a
  specialization of integral constant.

  @{
 */
using true_type = bool_type<true>;
using false_type = bool_type<false>;
//! @}

template<typename B, typename D>
class IsDerived{
private:
  //! A function returning true
  static true_type test(B* );
  //! A function returning false
  static false_type test(...);
  /*! A function that fails if D does not derive from B
    The second statement in the body of the function is
    needed to avoid warnings about undefined variables
  */
public:
  //! type is true_type or false_type according to the result of the test
  using type = decltype( test(static_cast<D*>(0) ) );
  //! value is true or false according to the result of the test
  static constexpr bool value = type::value;
};



#endif /* ISDERIVED_HPP_ */
