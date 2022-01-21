#ifndef __HH_POINT_HH__
#define __HH_POINT_HH__
#include <array>
#include <type_traits>
namespace apsc::Geometry
{

 /*!
   * @class Point
  * @brief A class that represents a point
  *
  * @tparam Value The type of the point coordinates
  * @tparam N The dimension of the space
  */
template <typename Value, std::size_t N>
class Point
  {
  public:
    using value_t = Value;
    static constexpr  std::size_t Dim = N;

    //! constructor
    Point()=default;
    //! constructor taking the coordinates of the point
    Point(std::array<Value, N> const &c) : M_coordinates{c} {};

    Value
    operator[](std::size_t i) const
    {
      return M_coordinates[i];
    }

    Value &
    operator[](std::size_t i)
    {
      return M_coordinates[i];
    }

  private:
      // Initialize with zero
    std::array<Value, N> M_coordinates={};
  };

//! A function for a Point
/*!
  It works for any Point provided that:
  \l the type of the coordinates is the same
  \li the dimension is the same
*/
template <class PointExt1, class PointExt2>
typename PointExt1::value_t
dot(PointExt1 const &a, PointExt2 const &b)
{
  static_assert(std::is_same<typename PointExt1::value_t,
                             typename PointExt2::value_t>::value,
                "dot error: Types must be the same");

  static_assert(PointExt1::Dim == PointExt2::Dim,
                "dot error: Dimension must be the same");

  typename PointExt1::value_t result(a[0] * b[0]);
  for(std::size_t i = 1; i < PointExt1::Dim; ++i)
    result += a[i] * b[i];
  return result;
}
} // namespace Geometry
#endif
