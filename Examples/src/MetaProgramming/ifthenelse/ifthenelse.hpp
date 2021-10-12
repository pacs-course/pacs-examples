#ifndef IFTHENELSE_HPP
#define IFTHENELSE_HPP
/*! @file ifthenelse.hpp
 This example is useful only to understand metaprogramming.
 The C++11 standard library contains the trait std::conditional
 that does exactly the same thing
*/
namespace apsc::TypeTraits
{
//! primary template: assumes the the boolean is true
/*!
 * @note the third template parameter is not indicated becouse not
 * used. It is needed only to have a proper priamary template for the
 * specialization. Indeed I could have also written template<bool C,
 * typename Ta, typename Tb> but there is no need to indicate Tb since
 * the primary tempalte does not use it
 */
template <bool C, typename Ta, typename> struct IfThenElse
{
  using type = Ta;
};

//! partial specialization: false yield third argument
template <typename Ta, typename Tb> struct IfThenElse<false, Ta, Tb>
{
  using type = Tb;
};

//! A useful type alias.
/*!
 Since c++14 you have one for all standard type traits returning a type.
*/
template <bool C, class Ta, class Tb>
using IfThenElse_t = typename IfThenElse<C, Ta, Tb>::type;

/* A different implementation that makes use of inheritance and of an
   helper template. (I call it If_then_else to differentiate with the
   former implementation)
*/

//! A helper class template
/*!
  It returns the type of the template argument
  It may seem a useless toy, but...it is in fact a map
  type -> type.

 */
template <class T> struct is_same_type
{
  using type = T;
};

//! primary template: assumes the the boolean is true
template <bool, typename Ta, typename> struct If_then_else : is_same_type<Ta>
{};

//! partial specialization: false yields third argument
template <typename Ta, typename Tb>
struct If_then_else<false, Ta, Tb> : is_same_type<Tb>
{};

} // namespace apsc::TypeTraits
//
#endif // IFTHENELSE_HPP
