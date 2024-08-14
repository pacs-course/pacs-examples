#ifndef HH_METADOT_HH
#define HH_METADOT_HH
#include "is_complex.hpp"
#include <array>
#include <complex>
#include <cstddef>
#include <type_traits>

/*!
 * It computes the dot product of two arrays of floating points or std::complex numbers!
 * @tparam N The size of the array
 * @tparam T1 The type stored in the first array
 * @tparam T2 The type stored in the second array
 * @param a The fist array
 * @param b The second array
 * @return  The common type between T1 and T2
 * @note You may decide to make a version that works with std::vector, but it is
 * not as efficient as this one that can be resolved compile time, and it cannot be constexpr.
 */
namespace apsc::Operators
{
  //@note you may decide to leave in the public interface only the * operator and hide this function
  //      in an anonymous namespace to make it a local function non usable from outside
template <std::size_t N, typename T1, typename T2>
constexpr auto
metadot(std::array<T1, N> const &a, std::array<T2, N> const &b) noexcept
{
  using result_type = std::common_type_t<T1, T2>;
  static_assert(std::is_arithmetic_v<result_type> ||
                    apsc::TypeTraits::is_complex_v<result_type>,
                  "Metadot:: The arrays must contain arithmetic values or "
                  "complex numbers\n"); // you may create a concept for this
  // casting 0 not really needed, but better be sure
  result_type result = static_cast<result_type>(0);
  // is integer or floating point
  if constexpr(std::is_arithmetic_v<result_type>)
    {
      for(std::size_t i = 0; i < N; ++i)
        result += a[i] * b[i];
    }
  else if constexpr(apsc::TypeTraits::is_complex_v<result_type>)
    {
      for(std::size_t i = 0; i < N; ++i)
        result += a[i] * std::conj(b[i]);
    }
  return result;
}

//    A version of * operator that uses metadot
   template<std::size_t N,typename T1, typename T2>
  constexpr auto operator * (std::array<T1,N>const & a,std::array<T2,N> const &
  b) noexcept
  {
    return metadot(a,b);
  }
} // end namespace apsc::Operators

//
#endif
