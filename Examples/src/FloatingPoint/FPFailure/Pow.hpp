#ifndef HH___POW__HH
#define HH___POW__HH
#include <type_traits>
/*!
  \file Pow.hpp
  
  This implementation works also if the argument x is not a
  constant expression. The optimization constant expression arguments
  is up to the compiler since constexpr is only a "suggestion" that the
  function may be computed at compile time if the argument is a constan expression.
  
  Note the use of std::integral_constant to select the correct overloaded
  function via tag dispatching.
*/
namespace apsc
{
  namespace implementation
  {
    // To use in tag dispatching
    template<int N>
    using IntToType=std::integral_constant<int,N>;
    
    //! Overload for N=0
    template<class R>
    constexpr R pow(const R x, IntToType<0>)
    {
      return 1;
    }

    //! Recursive call
    template<int N, class R>
    constexpr R pow(const R x, IntToType<N>)
    {
      return pow(x, IntToType<N-1>{}) * x;
    }
    
    
  }  // end namespace implementation
  
  //! the actual function
  template<int N, class R>
  constexpr R pow(const R x)
  {
    return implementation::pow(x, implementation::IntToType<N>{} );
  }

}// end namespace apsc

#endif
