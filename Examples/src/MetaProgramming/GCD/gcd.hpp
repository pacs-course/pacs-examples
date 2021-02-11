#ifndef HH_GCDXXX_HH
#define HH_GCDXXX_HH
#include <type_traits>
/*!
  @file gcd.hpp  greatest common divisor
 */
namespace apsc
{
namespace Utility
{
  /*!
   * A first example of computation of the greatest common divisor of two integers
   * in a static way (i.e. at compile time) using template specialization
   *
   * Gcd<M,N>::value returns the greatest common divisor of M and N
   * @tparam M The first integral value
   * @tparam N The second integral value
   */
  template <long unsigned int M, long unsigned int N>
  struct Gcd
  {
    static_assert(M>=N," First template argument cannot be smaller than the second");
    static constexpr long unsigned int  value = Gcd<N,M%N>::value;
  };
  //! Specialization to close recursion
  template <long unsigned int M>
  struct Gcd<M,0ul>
  {
    static_assert(M!= 0ul, "gcd between 0 and 0 not allowed");
    static constexpr long unsigned int value = M;
  };

  /*!
   * A template variable to imitate a feature of c++17
   * Gcd_v<M,N> is a variable equal to the greater common divisor between M and N
   */
  template <long unsigned int M, long unsigned int N>
  constexpr long unsigned int Gcd_v = Gcd<M,N>::value;


 //! Another implementation, it makes use of the
 //! new constexpr magic adn works with any integer types. Needs at least C++11
 //! @note Since c++17 you can use std::gcd, much better. This is only an example!
 template <class M, class N>
 auto constexpr  GCD(const M m , const N n) -> typename std::common_type<M,N>::type
  {
    return (n==0) ? m : GCD(n,m%n);
  }   
  
}
}
#endif
