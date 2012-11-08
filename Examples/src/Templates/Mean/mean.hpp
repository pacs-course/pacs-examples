#ifndef _MEANEXAMPLE_H
#define _MEANEXAMPLE_H
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
  
}
#endif
