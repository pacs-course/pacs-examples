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
   * A first example of computation of the greatest common divisor of two
   * integers in a static way (i.e. at compile time) using template function overloading
   *
   * Gcd<10,3>() returns greater common divisor between 10 and 3
   */
  template <unsigned M, unsigned N>
  unsigned Gcd()
  {
    static_assert(M >= N,
                  " First template argument cannot be smaller than the second");
    if constexpr (N == 0u)
        return M;
    else
      return Gcd<N, M % N>();
  };


  //! Another implementation, it makes use of the
  //! constexpr magic and works with any integer types.
  //! @note Since c++17 you can use std::gcd, much better. This is only an
  //! example!
  template <class M, class N>
  auto constexpr GCD(const M m, const N n) ->
    typename std::common_type<M, N>::type
  {
    return (n == 0) ? m : GCD(n, m % n);
  }

} // namespace Utility
} // namespace apsc
#endif
