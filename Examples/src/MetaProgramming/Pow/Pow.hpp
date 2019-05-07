#ifndef HH___POW__HH
#define HH___POW__HH
#include <type_traits>
//! A simple example of Pow for integer exponent
//! Implementation using template structs (it works also on C++98)
//! Primary template
template <long int X, unsigned int N>
struct Pow
{
  //! the type ised for the value
  using value_type = long int;
  //! The stored value
  const static value_type value = X * Pow<X,N-1>::value;
  //! Conversion operator
  operator value_type()const {return value;}
};

//! SPecialization for zero power
template <long int X>
struct Pow<X,0>
{
  using value_type = long int;
  const static value_type value = 1l;
  operator value_type()const {return 1l;}
};


//! A version that uses constexpr functions (C++11 onwards)
//! I use a namespace to hide implementation (NOT STRICTLY NECESSARY)
namespace implementation
{
  //! Implementation using constexpr functions
  /*!  This implementation works also if the argument x is not a
    integral constant.  The optimization is up to the compiler since
    constexpr is only a "suggestion" to the compiler that the
    expression may be optimized if the argument is a literal or a
    constant value.
    
    Note the use of integral_constant to select the correct overloaded
    function via tag dispatching.

    This example wants to show that by using the new constexpr
    construction one may in some cases avoid template metaprogramming.
    
  */
  // A class templates parametrized by an int
  /*
  template <unsigned int J>
  struct
  IntToType{};
  */
  // In c++11 I can use std::integral_constant
  template<int N>
  using IntToType=std::integral_constant<int,N>;
  
  //! Overload for N=0
  template<class R>
  constexpr R pow(const R x, IntToType<0>)
  {
    return 1;
  }

  template<int N, class R>
  constexpr R pow(const R x, IntToType<N>)
  {
    return pow(x, IntToType<N-1>{}) * x;
  }
  
  
}  
//! the actual function

template<int N, class R>
constexpr R pow(const R x)
{
  return implementation::pow(x, implementation::IntToType<N>{} );
}

//! In C++11 can have also a even simpler implementation
/*!
 *  But beware that constexpr functions are tricky. They work only on
 *  constant expressions. So the previous version (pow) is better!
 */

template<class R>
constexpr R POW(const R  x, const unsigned int N)
{
  return (N==0u) ? R(1): x*POW(x,N-1);
}


#endif
