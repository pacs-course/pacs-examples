#ifndef HH_METADOT_HH
#define HH_METADOT_HH
#include <array>
#include <cstddef>

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
  static T apply(std::array<T,N>const & a,std::array<T,N> const & b)
  {
    return a[M-1]*b[M-1] + metaDot<M-1>::apply(a,b);
  }
};

//! Specialization for the first element.
template<> struct metaDot<1>
{
  template<std::size_t N,typename T>
  static T apply(std::array<T,N>const & a,std::array<T,N> const & b)
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

#endif
