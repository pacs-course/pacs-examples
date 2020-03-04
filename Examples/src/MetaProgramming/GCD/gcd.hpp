#ifndef HH_GCDXXX_HH
#define HH_GCDXXX_HH
#include <type_traits>
/*!
  @file gcd.hpp  greatest common divisor
 */
namespace Utility
{
  //! Primary template. Uses recursion.
  /*!
   * Gdc<M,N> returns the greatest common divisor between N and M
   * This version works also with standard older than c++11
   */
  template <long unsigned int M, long unsigned int N>
  struct Gcd
  {
    static_assert(
                  M>=N,
                  " First template argument cannot be smaller than the second"
                  );
    static constexpr long unsigned int  value = Gcd<N,M%N>::value;
  };
  //! Specialization to close recursion
  template <long unsigned int M>
  struct Gcd<M,0ul>
  {
    static_assert(M!= 0ul, "gcd between 0 and 0 not allowed");
    static constexpr long unsigned int value = M;
  };

 
 using int_type=long unsigned int; 
 //! Another implementation, it makes use of the
 //! new constexpr magic adn works with any integer types!. Needs at least C++11
 //! @note In c++17 you can simplify a little the syntax (or use std::gcd() directly!)
 template <class M, class N>
 auto constexpr  GCD(const M m , const N n) -> typename std::common_type<M,N>::type
  {
    return (n==0) ? m : GCD(n,m%n);
  }   
  
}

#endif
