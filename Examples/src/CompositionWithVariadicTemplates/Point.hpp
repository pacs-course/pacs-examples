#ifndef __HH_POINT_HH__
#define  __HH_POINT_HH__
#include <array>
#include <type_traits>
//! A simple extendable class that represents a point
template<typename Value, std::size_t N, typename... Extensions>
class Point : public Extensions...
{
  std::array<Value,N> M_coordinates;
public:
  using value_t=value;
  static const std::size_t Dim=N;
  
  Value operator[](std::size_t i) const
  {
    return M_coordinates[i];
  }

  Value & operator[](std::size_t i) 
  {
    return M_coordinates[i];
  }
  
  //! Complex part: a constructor taking the composites
  /* 
     Skip reading it the first time
   */
  template<typename D=typename std::enable_if<sizeof...(Extensions)!=0,void>,typename ...T>
    Point(T&& ...ext):Extensions(std::forward<T>(ext))...{};
  //! Since I have defined a constructor I need to indicate the default
  Point()=default;
};


//! A function for a Point
/*! 
  It works for any composite Point provided that:
  \l the type of the coordinates is the same
  \li the dimension is the same
*/
template <class PointExt1, class PointExt2>
typename PointExt1::value_t dot(PointExt1 const &a, PointExt2 const & b)
{
  static_assert(std::is_same<typename PointExt1::value_t,typename PointExt2::value_t>::value,
		"dot error: Types must be the same");
  
  static_assert(PointExt1::Dim==PointExt2::Dim,
		"dot error: Dimension must be the same");
  
  typename PointExt1::value_t result(a[0]*b[0]);
  for (std::size_t i=1; i< PointExt1::Dim;++i)result+=a[i]*b[i];
  return result;
}

#endif
