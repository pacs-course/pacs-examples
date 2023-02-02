//
// Created by forma on 30/01/23.
//

#ifndef C_INTRIANGLE_HPP
#define C_INTRIANGLE_HPP
#include <array>
#include <cmath>
#include <concepts>
#include <vector>

namespace apsc
{
/*!
 * Concept to express a simple basic structure for a polygon
 */
template <class P>
concept BarePolygon = requires(P p) {
                        // address operator returns something convertible to an
                        // array of 2 doubles
                        {
                          P{}[0]
                        } -> std::convertible_to<std::array<double, 2>>;
                        p.size();
                      };

/*!
 * Template function to identify if a point is inside a polygon
 * The polygon is defined by its vertexes, oriented either clockwise or
 * anticlockwise.
 *
 * It works by testing whether the point stays always on the same side of the
 * polygon edges. Note that a version that assumes a specific orientation can be
 * slightly more efficient.
 *
 * @tparam Poly A structure so that [i] returns a std::array<double,2>
 * @param poly A structure so that [i] returns a std::array<double,2> and
 * [i][j] returns the j-th coordinate of the i--th vertex
 * @param point The point to check.
 */
template <BarePolygon Poly>
bool inPolygon(Poly const &poly, std::array<double, 2> const &point);
/*!
 * Template function to identify if a point is inside a polygon
 * The polygon is defined by its vertexes, oriented either clockwise or
 * anticlockwise.
 *
 * It works by testing if the horizontal line has an  even number of
 * intersection with the edges
 *
 * @tparam Poly A structure so that [i] returns a std::array<double,2>
 * @param poly A structure so that [i] returns a std::array<double,2> and
 * [i][j] returns the j-th coordinate of the i--th vertex
 * @param point The point to check.
 */

template <BarePolygon Poly>
bool inPolygonFast(Poly const &poly, std::array<double, 2> const &point);

// IMPLEMENTATIONS---------------------------------------------------------

template <BarePolygon Poly>
bool
inPolygon(Poly const &poly, std::array<double, 2> const &point)
{
  using Point = std::array<double, 2>;
  // the vector b-a stored in an array
  auto diff = [](Point const &a, Point const &b) -> Point {
    return {b[0] - a[0], b[1] - a[1]};
  };

  // Dot producto of two arrays
  auto dot = [](Point const &a, Point const &b) {
    return b[0] * a[0] + b[1] * a[1];
  };

  // Get the vector defining the normal to the  i-th edge of the polygon
  auto nside = [&poly](std::size_t i) -> Point {
    std::size_t j = (i + 1u) % poly.size();
    return {poly[j][1] - poly[i][1], poly[i][0] - poly[j][0]};
  };
  auto sign = dot(nside(0), diff(point, poly[0]));
  for(std::size_t i = 1u; i < poly.size(); ++i)
    {
      if(dot(nside(i), diff(point, poly[i])) * sign <= 0.0) // opposite sign
        return false;
    }
  return true;
}
template <BarePolygon Poly>
bool
inPolygonFast(Poly const &poly, std::array<double, 2> const &point)
{
  using Point = std::array<double, 2>;
  // the vector b-a stored in an array
  auto halfspace = [&point](Point const &a) { return a[0] >= point[0]; };

  // Dot producto of two arrays
  auto edgeValid = [&point, &halfspace](Point const &a, Point const &b) {
    return halfspace(a) or halfspace(b);
  };

  // Get the vector defining the normal to the  i-th edge of the polygon
  auto intersectHorizontalHalfPlane = [&point, &edgeValid](Point const &a,
                                                           Point const &b)
  {
    bool ok=false;
    if (edgeValid(a, b))
      {
        if( (a[1] < point[1] and b[1] >= point[1]) or
             (b[1] < point[1] and a[1] >= point[1])
           )
          {
            auto xp=((point[1]-a[1])*b[0]+(b[1]-point[1])*a[0])/(b[1]-a[1]);
            ok= xp>=point[0];
          }
      }
    return ok;
  };

  unsigned count = 0u;
  for(std::size_t i = 0u; i < poly.size(); ++i)
    {
      auto a = poly[i];
      auto b = poly[(i + 1u) % poly.size()];
      if(intersectHorizontalHalfPlane(a, b))
        ++count;
    }
  return count % 2 == 1;
}
} // namespace apsc
#endif // C_INTRIANGLE_HPP
