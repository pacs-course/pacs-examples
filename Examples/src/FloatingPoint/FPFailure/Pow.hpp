#ifndef HH___POW__HH
#define HH___POW__HH
#include <type_traits>
/*!
  \file Pow.hpp

  This implementation works also if the argument x is not a
  constant expression. The optimization constant expression arguments
  is up to the compiler since constexpr is only a "suggestion" that the
  function may be computed at compile time if the argument is a constan
  expression.

  Note the use of std::integral_constant to select the correct overloaded
  function via tag dispatching.
*/
namespace apsc
{
template <int N, class R>
constexpr R
pow(const R x)
{
  if constexpr(N == 0)
    {
      return 1;
    }
  else if constexpr(N == 1u)
    {
      return x;
    }
  else
    {
      return x * pow<N - 1u>(x);
    }
}

} // end namespace apsc

#endif
