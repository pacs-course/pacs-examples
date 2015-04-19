#ifndef _MEANEXAMPLE_H
#define _MEANEXAMPLE_H
#include <type_traits>
namespace Utility{
  template <class T>
  inline T  mean (T const & a, T const & b)
  {
    return  0.5*(a+b);
  }
  
  template <class T>
  inline T mean (T* const & a, T* const & b)
  {
    return  0.5*(*a+*b);
  }
  
  // With variadic templates (only for nerds!)
  template <typename T>
  T Sum(const T & a)
  {
    return a;
  }

  template <typename T, typename... Ts>
  typename std::common_type<T,Ts...>::type Sum(const T& a, const Ts&... Args)
  {
    return a + Sum(Args...);
  }
}
#endif
