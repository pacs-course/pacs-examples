//
// Created by forma on 03/04/23.
//

#ifndef EXAMPLES_EDGEGEO_HPP
#define EXAMPLES_EDGEGEO_HPP
#include <array>
namespace apsc::Geometry
{
namespace concepts
{
  /*!
  Expresses the semantic of a 2D point
  */
  template <typename P>
  concept Point2D = requires(P p) {
    P{0., 0.}; // constructible from two doubles
    { p[0] } -> std::convertible_to<double>;
    { p[1] } -> std::convertible_to<double>;
  };
} // namespace concepts
//! A 2D point that satisfies that semantic
using Point = std::array<double, 2>;
//! Simple class holding an edge
template <concepts::Point2D POINT = Point> class EdgeGeo
{
public:
  using Point_t = POINT;
  // Constructor giving end points
  EdgeGeo(const POINT &left, const POINT &right) : M_points{left, right} {};
  //! Set end points
  void
  set(POINT &&left, POINT &&right)
  {
    M_points = {left, right};
  };

  POINT
  operator[](std::size_t i) const { return M_points[i]; };

  POINT &
  operator[](std::size_t i) noexcept
  {
    return M_points[i];
  }

private:
  std::array<POINT, 2> M_points{{{0., 0.}, {0., 0.}}};
};
namespace concepts
{
  /*!
   * Expresses the basic semantic of a 2D edge
   * @tparam E the edge
   * @details
   * The concept should express that E has operator[i] that returns
   * a 2D point (satisfying the Point2D concept)
   */
  template <typename E>
  concept Edge = requires(E const e) {
    typename E::Point_t;
    requires concepts::Point2D<typename E::Point_t>;
    { e[0] } -> concepts::Point2D;
    { e[1] } -> concepts::Point2D;
  };
} // namespace concepts
} // namespace apsc::Geometry
#endif // EXAMPLES_EDGEGEO_HPP
