#ifndef __GEO_HH_
#define __GEO_HH_
#include <array>
namespace Geometry2
{
//! the base class of visitors
class ShapeVisitor; // FORWARD DECLARATION

//! a class of elements that supports the visitor pattern
/*!
 * This hierarchy of classes may be visited using the visitor design pattern
 * The visitor must have a method called visit(concreteShape &) that visit a
 * concrete object of the hierarchy and operates on it accordingly. I use
 * overloading to select the concrete visitor.
 */
class Shape
{
public:
  virtual double measure() const = 0;
  virtual ~Shape() = default;
};

//! A class representing a point in 2D
class Point : public Shape
{
public:
  static int const myDim = 0;

  explicit Point(double x = 0, double y = 0)
    : M_coor{x, y} {}; // builds a Point
  Point() = default;

  void setCoordinates(double x, double y); // Sets point coordinates

  // We can get the coordinates with  operator []
  double
  operator[](int i) const
  {
    return M_coor[i];
  }
  double &
  operator[](int i)
  {
    return M_coor[i];
  }

  friend Point operator+(const Point &, const Point &);
  friend Point operator-(const Point &, const Point &);
  Point        operator*(const double &) const;
  friend Point operator*(const double &, const Point &);

  double
  measure() const override
  {
    return 0.;
  }

private:
  double M_coor[2] = {0., 0.};
};

/*!
 * A class representing a Triangle.
 *
 * The triangle vertices are stored as Points.
 */
class Triangle : public Shape
{
public:
  static int const myDim = 2;
  static const int numVertices = 3;
  static const int numSides = 3;

  Triangle(Point const &a, Point const &b, Point const &c) : M_points{a, b, c}
  {} //!< Points are given

  //! Change a point
  /*!
   * @param i The local index of the point in the triangle
   * @param p The new point
   * @return  The stored point (before substitution)
   */
  Point changePoint(int i, Point const &p);
  //! Extract a point
  Point
  operator[](int i) const
  {
    return M_points[i];
  }
  //! Extract a point
  Point &
  operator[](int i)
  {
    return M_points[i];
  }
  //! Extract all Points
  std::array<Point, numVertices> &
  getPoints()
  {
    return M_points;
  }
  double       measure() const override;           //!< Triangle area
  Point &      edgePoint(int edgenum, int endnum); //!< The point on an edge
  Point const &edgePoint(int edgenum, int endnum) const; //!< The const version

  static int edge(int edgenum, int endnum); //!< The edge numbering

private:
  std::array<Point, numVertices> M_points;
  static int const               M_edge[numSides][2];
};

class Quadrilateral : public Shape
{
public:
  static int const myDim = 2;
  static const int numVertices = 4;
  static const int numSides = 4;

  Quadrilateral(Point const &a, Point const &b, Point const &c, Point const &d)
    : M_points{a, b, c, d} {};
  //! Change a point
  /*!
   * @param i The local index of the point in the triangle
   * @param p The new point
   * @return  The stored point (before substitution)
   */
  Point changePoint(int i, Point const &p);
  //! Extract a point
  Point
  operator[](int i) const
  {
    return M_points[i];
  }
  //! Extract a point
  Point &
  operator[](int i)
  {
    return M_points[i];
  }
  //! Extract all points
  std::array<Point, numVertices> &
  getPoints()
  {
    return M_points;
  }

  Point &      edgePoint(int edgenum, int endnum); //!< The point on an edge
  Point const &edgePoint(int edgenum, int endnum) const; //!< The const version

  static int edge(int edgenum, int endnum); // The edge numbering

  double measure() const override; // area

private:
  std::array<Point, numVertices> M_points;
  static int const               M_edge[numSides][2];
};
} // namespace Geometry2
#endif
