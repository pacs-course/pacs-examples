#ifndef HH_POLYGON_HH
#define HH_POLYGON_HH
#include <iostream>
#include <vector>
namespace Geometry
{
using ::std::cout;
using ::std::endl;
using ::std::ostream;
using ::std::vector;

/*!
 * @brief Two-dimensional point used to store polygon vertices.
 */
struct Point2D
{
  double x;
  double y;
};

//! Euclidean distance between two points.
double distance(Point2D const &a, Point2D const &b);

/*!
 * @brief Container type used to store polygon vertices.
 */
typedef vector<Point2D> Vertices;

/*!
 * @brief Base class of the polygon hierarchy used in the plugin example.
 *
 * The geometric functionality is intentionally minimal because the focus of
 * the example is the plugin architecture rather than computational geometry.
 * Derived classes mainly override `showMe()` so that the runtime-selected
 * object can identify itself.
 */
class Polygon
{
public:
  //! Builds an empty polygon and optionally marks it as convex.
  explicit Polygon(bool convex = false);

  //! Builds a polygon from a list of vertices and a convexity flag.
  explicit Polygon(Vertices v, bool convex = false);

  //! Virtual destructor required by the polymorphic hierarchy.
  virtual ~Polygon();

  //! Replaces the list of vertices.
  void            setVertices(Vertices const &v);

  //! Returns whether the polygon is marked as convex.
  bool            isConvex() const;

  //! Returns the stored vertices.
  Vertices const &theVertices() const;

  //! Prints a textual description of the polygon.
  virtual void    showMe(ostream &out = cout) const;

  //! Placeholder for a diameter computation.
  virtual double diameter() const;

  //! Placeholder for an inscribed-circle computation.
  virtual double inscibedCircle() const;

  //! Placeholder translation operation.
  void translate(double const &x, double const &y);

  //! Placeholder rotation operation.
  void rotate(double const &angle);

  //! Placeholder scaling operation.
  void scale(double const &xfactor, double const &yfactor);

protected:
  //! Convexity flag associated with the polygon.
  bool     isconvex;

  //! Stored polygon vertices.
  Vertices vertexes;
};
} // namespace Geometry

#endif
