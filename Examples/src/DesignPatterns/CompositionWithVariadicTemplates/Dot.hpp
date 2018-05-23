#ifndef HH_DOT_HH
#define HH_DOT_HH
#include "Point.hpp"
template<typename PointExt>
struct Dot
{
  typename PointExt::value_t dot(PointExt const & a, PointExt const & b)
  {
    PointExt::value_t result=a[0]*b[0];
    for (std::size_t i=1; i<PointExt::Dim;++i)result+=a[i]*b[i];
    return result;
  }
}
#endif
