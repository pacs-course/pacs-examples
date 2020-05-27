#ifndef HH_IS_COMPLEX_HHH
#define HH_IS_COMPLEX_HHH
#include <type_traits>
#include <complex>
namespace TypeTraits
{
  template<class T>
  struct is_complex : public std::false_type
  {};

  template<class T>
  struct is_complex<std::complex<T>> : public std::true_type
  {};
  
}
#endif
