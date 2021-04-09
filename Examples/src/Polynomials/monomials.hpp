#ifndef HH__MONOMIALS__HH
#define HH__MONOMIALS__HH
#include<type_traits>
namespace apsc::LinearAlgebra
{
  namespace implementation
  {
    //! Implementation using constexpr functions
    /*!  This implementation works also if the argument x is not a
      integral constant.  The optimization is up to the compiler since
      constexpr is only a "suggestion" to the compiler that the
      expression may be optimized if the argument is a constant expression.
      
      Note the use of integral_constant to select the correct overloaded
      function via tag dispatching.
    */
    template<int N>
    using IntToType=std::integral_constant<unsigned int,N>;
    
    //! Overload for N=0
    template<class R>
    constexpr R monomial(const R , IntToType<0>)
    {
      return 1;
    }

    template<unsigned int N, class R>
    constexpr R monomial(const R x, IntToType<N>)
    {
      return monomial(x, IntToType<N-1>()) * x;
    }
    
  }  
  //! the actual function
  
  template<unsigned int N, class R>
  constexpr R monomial(const R x)
  {
    return implementation::monomial(x, implementation::IntToType<N>{} );
  }
}
#endif
