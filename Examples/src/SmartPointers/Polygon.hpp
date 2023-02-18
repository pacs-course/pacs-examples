#ifndef HH_POLYGON_HH
#define HH_POLYGON_HH
#include <array>
#include <iostream>
#include <utility>
#include <vector>

/*!  @file Polygon.hpp
  @briefThis is an example of class hierarchy that
  represents poligonal object.

  @detail This module is wrapped in the namespace `Geometry`. It
  represent a first example of class hyerarchy with public
  inheritance. The class `AbstractPolygon` defines the general public
  interface of all other classes representing polygonal objects.

  We make use of some syntax of the C++11 new standard so you need to
  compile with *-std=c++0x* or (since gcc 4.7) *-std=c++11*
   */
namespace Geometry
{
//! A class that holds 2D points
class Point2D
{
public:
  using Coord = std::array<double, 2>;
  //! Constructor giving coordinates.
  Point2D(double xx = 0.0, double yy = 0.0) : coor{xx, yy} {}
  //! Constructor that takes an array
  Point2D(Coord const &c) : coor{c} {}
  //! Returns coordinates in an array
  auto
  get() const
  {
    return coor;
  }
  //! Sets point coordinates
  void
  set(double const xx, double const yy)
  {
    coor = {xx, yy};
  }
  //! A setter that takes an array
  void
  set(Coord const &c)
  {
    coor = c;
  }
  //! x coordinate
  double
  x() const
  {
    return coor[0];
  }
  //! y coordinate
  double
  y() const
  {
    return coor[1];
  }
  //! x coordinate, to write on
  double &
  x()
  {
    return coor[0];
  }
  //! y coordinate, to write on
  double &
  y()
  {
    return coor[1];
  }
  //! Subtraction is implemented as an external friend function
  friend Point2D operator-(Point2D const &a, Point2D const &b);
  //! Addition is implemented as an external friend function
  friend Point2D operator+(Point2D const &a, Point2D const &b);

private:
  Coord coor;
};

//! subtraction operator.
/*!
  It is defined in the header file because I want it to be
  inline.
*/
inline Point2D
operator-(Point2D const &a, Point2D const &b)
{
  return Point2D{a.coor[0] - b.coor[0], a.coor[1] - b.coor[1]};
}

//! Addition operator.
/*!
  It is defined in the header file because I want it to be
  inline.
*/
inline Point2D
operator+(Point2D const &a, Point2D const &b)
{
  return Point2D{a.coor[0] + b.coor[0], a.coor[1] + b.coor[1]};
}

//! Distance between points
double distance(Point2D const &a, Point2D const &b);

//! Defines the common interface of polygons.
class AbstractPolygon
{
public:
  using Vertices = std::vector<Point2D>;
  //! Defaulted constuctor
  AbstractPolygon()=default;
  //! virtual destructor (is a base class)
  virtual ~AbstractPolygon() = default;
  //! Returns the number of vertices.
  /*!  We return Vertices::size_type and not just int because
    size_type is guaranteed to be the correct type for indexes in
    stl vectors. Its actual type may be implementation dependent.
    In this case, however, int would have been fine (size_type is
    guaranteed to be an integral type, more precisely
    a type convertible to unsigned int).
  */
  Vertices::size_type
  size() const
  {
    return vertexes.size();
  }
  //! Is the polygon convex?
  bool
  isConvex() const
  {
    return isconvex;
  }
  //! Returns the vertices (read only)
  Vertices const &
  theVertices() const
  {
    return vertexes;
  }
  //! Outputs some info on the polygon
  virtual void showMe(std::ostream &out = std::cout) const;
  //! The area of the polygon (with sign!)..
  virtual double area() const = 0;

  AbstractPolygon(Vertices const &v) : vertexes(v) { checkConvexity(); }
  // will be redefined
  AbstractPolygon(Point2D const &, double, double){};

protected:

  Vertices vertexes={};
  bool     isconvex = false;
  //! Test convexity of the polygon
  void checkConvexity();
};

//! Class for a generic Polygon
/*
  A generic Polygon is defined by a set of Vertices which are
  provided by the user
 */
class Polygon : public AbstractPolygon
{
public:
  using AbstractPolygon::AbstractPolygon;
  /*!
    The area is positive if vertices are given in
    counterclockwise order
   */
  virtual double area() const override;
  //! Set vertices for a Polygon.
  void set(Vertices const &v);
  //! Specialised version for generic polygons.
  virtual void showMe(std::ostream &out = std::cout) const override;
  //! Strange destructor to show what happens if you uses smart ptrs
  ~Polygon() { std::cout << "Destroying a generic polygon" << std::endl; };
};

//! A square
/*!
  The square is a final class derived from polygon.
  The final keyword has been introduced in C++11
 */
class Square final : public AbstractPolygon
{
public:
  Square() { this->isconvex = true; };
  //! Special constructor valid only for squares.
  /*!
    /param origin Point which gives the first vertex of the square.
    /param length The length of the side.
    /param angle In radians, tells how the square is  rotated.
   */
  Square(Point2D const &origin, double length, double angle = 0.0);

  /*
   * Delegating constructor
   */
  Square(Vertices const & v):AbstractPolygon(v){};
  /*!
   * Constictor giving origin and dimensions
   *
   * @param origin Origin
   * @param length side length
   * @param angle rotation angle
   */
  void
  set(Point2D const &origin, double length, double angle = 0.0)
  {
    // I am lazy so I use the constructor and copy;
    *this = Square(origin, length, angle);
  }
  //! Specialised version for squares
  double area() const override;
  //! Strange destructor to show what happens if you uses smart ptrs
  ~Square() { std::cout << "Destroying a square" << std::endl; };
  //! Specialised version for squares.
  void showMe(std::ostream &out = std::cout) const override;
};

//! A triangle
class Triangle final : public AbstractPolygon
{
public:
  using AbstractPolygon::AbstractPolygon;

  Triangle() { this->isconvex = true; };
  //! Constructor for triangles.
  /*!
    It uses the general copy contructor but it verifies if
    we give 3 points, if not the program exits with exit(1)
    It may be an interesting case to implement exceptions
   */
  Triangle(Vertices const &);
  //! Specialised for Triangles
  double area() const override;
  //
  //! Strange destructor to show what happens if you uses smart ptrs
  ~Triangle() { std::cout << "Destroying a Triangle" << std::endl; };
  //! Specialised for Triangles
  void showMe(std::ostream &out = std::cout) const override;
};

} // namespace Geometry

#endif
