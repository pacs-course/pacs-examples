#ifndef HH_GCDXXX_HH
#define HH_GCDXXX_HH
/*!
  @file gcd.hpp  greatest common divisor
 */
namespace Utility
{
  //! Primary template. Uses recursion.
  template <long unsigned int M, long unsigned int N>
  struct Gcd
  {
    static_assert(
                  M>=N,
                  " First template argument cannot be smaller than the second"
                  );
    static constexpr auto value = Gcd<N,M%N>::value;
    
  };
  //! Specialization to close recursion
  //! Primary template. Uses recursion.
  template <long unsigned int M>
  struct Gcd<M,0ul>
  {
    static_assert(M!= 0ul, "gcd between 0 and 0 not allowed");
    static constexpr auto value = M;
  };

  //! If I do not know the values at compile time
  /*!  
    Implemented via recursion. However I suggest in general to
    avoid recursion (unless in metaprogramming) since it is not very
    efficient.  Moreover, if I deal with very big integers maybe I can
    exceed the stack capability for recursive call.
    Yet, the recurrsive implementation is very elegant, I admit!.
   */
  long unsigned int gcd(long unsigned int M, long unsigned int N);
  
}

#endif
