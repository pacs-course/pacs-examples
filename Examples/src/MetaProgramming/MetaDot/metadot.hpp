#ifndef HH_METADOT_HH
#define HH_METADOT_HH
#include "is_complex.hpp"
#include <array>
#include <complex>
#include <cstddef>
#include <type_traits>
/*! An example of template metaprogramming.

  It implements the dot product of two std::arrays.

  std::size_t returns the correct type used to dimension array. Since
  it can be implementation dependent (32 or 64 bits) it is better to
  use it instead of just unsigned int.

  The class can be further generalised.

*/
//! namespace to hide this helper functions
namespace apsc::internals
{
template<std::size_t M>
struct metaDot
{
  template<std::size_t N,typename T>
  static constexpr T apply(std::array<T,N>const & a,std::array<T,N> const & b)
  {
    return a[M-1]*b[M-1] + metaDot<M-1>::apply(a,b);
  }
};

//! Specialization for the first element.
template<> struct metaDot<1>
{
  template<std::size_t N,typename T>
  static constexpr T apply(std::array<T,N>const & a,std::array<T,N> const & b)
  {
    return a[0]*b[0];
  }
};
  
}// end namespace apsc::internals
namespace apsc::Operators
{
//! Operator overloading: this is what will be actually used.
template<std::size_t N,typename T>
T operator * (std::array<T,N>const & a,std::array<T,N> const & b)
{
  return apsc::internals::metaDot<N>::apply(a,b);
}

 /*!
  * A different implementation (since C++17). More flexible: it treats also complex numbers!
  * @tparam N The size of the array
  * @tparam T1 The type stored in the first array
  * @tparam T2 The type stored in the second array
  * @param a The fist array
  * @param b The second array
  * @return  The common type between T1 and T2
  *
  * @note You can overload operator * with this version, see commented version
  */
template<std::size_t N,typename T1, typename T2>
  constexpr auto metadot17(std::array<T1,N>const & a,std::array<T2,N> const & b) noexcept
  {
    using result_type=std::common_type_t<T1,T2>;
    // casting 0 not really needed, but better be sure
    result_type result=static_cast<result_type>(0);
    // is integer or floating point
    if constexpr (std::is_arithmetic_v<result_type>)
      {
        for (std::size_t i = 0; i<N; ++i) result+=a[i]*b[i];
      }
    else if constexpr (apsc::TypeTraits::is_complex_v<result_type>)
    {
      for (std::size_t i = 0; i<N; ++i) result+=a[i]*std::conj(b[i]);
    }
    else
      {
        static_assert(std::is_arithmetic_v<result_type>||apsc::TypeTraits::is_complex_v<result_type>, "Metadot17:: The arrays must contain arithmetic values or complex numbers\n");
      }
    return result;                     
  }
/*
    A version of * operator that uses metadot17
   template<std::size_t N,typename T1, typename T2>
  constexpr auto operator * (std::array<T1,N>const & a,std::array<T2,N> const & b) noexcept
  {
    return metadot17(a,b);
  }
 */
}// end namespace apsc::Operators

//
#endif
