#ifndef HH___POW__HH
#define HH___POW__HH
#include <type_traits>
//! Implementation using template structs
//! Primary template
template <long int X, unsigned int N>
struct Pow
{
  //! the type ised for the value
  using value_type = long int;
  //! The stored value
  const static value_type value = X * Pow<X,N-1>::value;
  //! Conversion operator
  constexpr operator value_type()const {return value;}
};

//! SPecialization for zero power
template <long int X>
struct Pow<X,0>
{
  using value_type = long int;
  const static value_type value = 1l;
  constexpr operator value_type()const {return 1l;}
};


//! namespace to hide implementation (NOT STRICTLY NECESSARY)
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
  // In c++11 I ca use std::integral_constant
  template<unsigned int N>
  using IntToType=std::integral_constant<unsigned int,N>;

  template<unsigned int N>
  constexpr long int pow(const long int x, implementation::IntToType<N>)
  {
    return pow(x, IntToType<N-1>{} ) * x;
  }
  //! Overload for N=0
  template<>
  constexpr long int pow(const long int  x, implementation::IntToType<0>)
  {
    return 1;
  }
  
}  
//! the actual function

  template<int N>
  constexpr long int pow(const long int  x)
  {
    return implementation::pow(x, implementation::IntToType<N>{} );
  }

#endif
