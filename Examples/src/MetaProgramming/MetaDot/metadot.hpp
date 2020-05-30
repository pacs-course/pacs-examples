#ifndef HH_METADOT_HH
#define HH_METADOT_HH
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
namespace internals
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
  
}
//! Operator overloading: this is what will be actually used.
template<std::size_t N,typename T>
T operator * (std::array<T,N>const & a,std::array<T,N> const & b)
{
  return internals::metaDot<N>::apply(a,b);
}

  //! C++14 onward version. You may use this if you prefer
  //! which is also more flexibe. You can convert it it operator*,
  //! since also operators can be constexpr
template<std::size_t N,typename T1, typename T2>
  constexpr auto metadot17(std::array<T1,N>const & a,std::array<T2,N> const & b)
  {
    using result_type=std::common_type_t<T1,T2>;
    // casting 0 not really needed
    result_type result=static_cast<result_type>(0);
    // is integer or floating point
    if constexpr (std::is_arithmetic<result_type>::value)
      {
        for (std::size_t i = 0; i<N; ++i) result+=a[i]*b[i];
      }
    else // I assume T2 is complex
    {
      for (std::size_t i = 0; i<N; ++i) result+=a[i]*std::conj(b[i]);
    }
    return result;                     
  }


//
#endif
