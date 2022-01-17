#ifndef HH_METADOT_HH
#define HH_METADOT_HH
#include "is_complex.hpp"
#include <array>
#include <complex>
#include <cstddef>
#include <type_traits>

/*!
 * It computes the dot roduct of two arrays of floating points or std::complex numbers!
 * @tparam N The size of the array
 * @tparam T1 The type stored in the first array
 * @tparam T2 The type stored in the second array
 * @param a The fist array
 * @param b The second array
 * @return  The common type between T1 and T2
 *
 * @note You can overload operator * with this version, see commented version
 */
namespace apsc::Operators
{
template <std::size_t N, typename T1, typename T2>
constexpr auto
metadot(std::array<T1, N> const &a, std::array<T2, N> const &b) noexcept
{
  using result_type = std::common_type_t<T1, T2>;
  static_assert(std::is_arithmetic_v<result_type> ||
                    apsc::TypeTraits::is_complex_v<result_type>,
                  "Metadot17:: The arrays must contain arithmetic values or "
                  "complex numbers\n");
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
/*
    A version of * operator that uses metadot17
   template<std::size_t N,typename T1, typename T2>
  constexpr auto operator * (std::array<T1,N>const & a,std::array<T2,N> const &
  b) noexcept
  {
    return metadot17(a,b);
  }
 */
} // end namespace apsc::Operators

//
#endif
