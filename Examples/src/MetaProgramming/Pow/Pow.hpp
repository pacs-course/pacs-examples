#ifndef HH___POW__HH
#define HH___POW__HH
#include <type_traits>
//! A version that uses constexpr functions (C++11 onwards) and tag dispatching
//! I use a namespace to hide implementation (NOT STRICTLY NECESSARY)
namespace implementation
{
  //! Implementation using constexpr functions
  /*!
    This implementation works also if the argument x is not a
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
  constexpr R pow(const R & x, IntToType<0>)
  {
    // I convert explicitly so R can be anything from which 1.0
    // convertible to R!
    return static_cast<R>(1.0);
  }

  template<int N, class R>
  constexpr R pow(const R & x, IntToType<N>)
  {
    return pow(x, IntToType<N-1>{}) * x;
  }
}  
//! the actual function

template<int N, class R>
constexpr R pow(const R& x)
{
  return implementation::pow(x, implementation::IntToType<N>{} );
}

//! Bust since c++17 we can get rid of tag dispatching and doing things more simply

template<int N, class R>
constexpr R pow17(const R & x)
{
  if constexpr (N==0)
      // I convert explicitly so R can be anything from which 1.0
      // convertible to R!
      return static_cast<R>(1.0);
  else
    return pow17<N-1>(x);
}


//! Since C++11 can have also a even simpler implementation
/*!
 *  The nice thing about this implementation is that it works even if
 *  the exponent is not a constant expression, so it is more flexible.
 *  If both arguments are constant expressions, the compiler may
 *  resolve the computation at compile time.
*/
template<class R>
constexpr R POW(const R  x, const unsigned int N)
{
  return (N==0u) ? static_cast<R>(1.0): x*POW(x,N-1);
}


#endif
