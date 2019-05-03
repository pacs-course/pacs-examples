#ifndef __HH_POINT_HH__
#define  __HH_POINT_HH__
#include <array>
#include <type_traits>
namespace Geometry
{
  // @todo This example can be done better by leaving Point as a simple
  // class and creating a Composer template class that composes by public
  // inheritance all template parameters.
  //
  //! A simple extendable class that represents a point
  template<typename Value, std::size_t N, typename... Extensions>
  class Point : public Extensions...
  {
    // implicitely private
    std::array<Value,N> M_coordinates;
  public:
    using value_t=Value;
    static const std::size_t Dim=N;
  
    Value operator[](std::size_t i) const
    {
      return M_coordinates[i];
    }

    Value & operator[](std::size_t i) 
    {
      return M_coordinates[i];
    }


    Point(Point const &)=default;
  
    //! Complex part: a constructor taking the point coordinates and composites objects
    /* 
       Skip reading it the first time
       enable_if is necessary because if I use point with zero or 1 argument this
       definition will fail.
    */
    template<typename COO, typename... T, typename std::enable_if_t<sizeof...(T)!=0,int> =0>
      Point(COO&& c, T&& ...ext):Extensions(std::forward<T>(ext))...,
      M_coordinates(std::forward<COO>(c))
      {};
    //! This if I want to pass just the coordinates (or if the point is simple)!
    Point(std::array<Value,N> const & c):M_coordinates{c}{};

    //! Since I have defined constructors I need to indicate the default
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
}// end namespace
#endif
