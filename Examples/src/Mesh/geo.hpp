#ifndef __GEO_HH_
#define __GEO_HH_
#include <array>
namespace Geometry
{
inline constexpr int ndim = 2;
//! A simple class to represent a point in 2D
/*!
  This class aims a representing a single point and provides some basic
  operation. /todo It may be templatized on the number of dimensions
 */
class Point
{
public:
  //! The intrinsic dimension of a Point
  static constexpr int myDim = 0;
  /*! \defgroup COnstructors Constructors and assigmement*/
  //! @{
  //! Builds a point from coordinates
  /*!
    It serves also ad default constructor
  */
  explicit Point(double x = 0, double y = 0) : M_coor{{x, y}} {}; // C+11
/*!
 * A point is convertible to an array of 2 doubles. Useful for handling point  simply.
 * @return An array containing the point coordinates
 */
  operator std::array<double,ndim>(){return M_coor;}
  /*!@}*/
  //! Destructor is virtual since I may want to use polymorphism
  virtual ~Point(){};
  //! Sets point coordinates
  void
  setCoordinates(double x, double y)
  {
    M_coor[0] = x;
    M_coor[1] = y;
  }
  //! Get points coordinates
  void
  getCoordinates(double &x, double &y) const
  {
    x = M_coor[0];
    y = M_coor[1];
  }
  //! We can get the coordinates also by operator []
  double
  operator[](int i) const
  {
    return M_coor[i];
  }
  //! We can get the coordinates also by operator []
  double &
  operator[](int i)
  {
    return M_coor[i];
  }
  /*!
    \defgroup Operators Operators on Points
    @{
  */
  friend Point operator+(const Point &, const Point &);
  friend Point operator-(const Point &, const Point &);
  Point        operator*(const double &) const;
  friend Point operator*(const double &, const Point &);
  /*!@}*/
protected:
  //! The raw storage
  std::array<double, ndim> M_coor;
};

// Class edge STILL TO BE DONE

//! A class representing a geometrical triangle
/*!
  I have made a choice: store the points as pointers to existing points.
  \li Advantages: I can address e list of existing points directly. If a modify
  a point also the triangle is modified. \li Disadvantages: The points must be
  passed as references or pointers. If a point addressed by a triangle is
  destroyed I have dangling pointers!

  Since the Point is just a base class that may be enriched by composition I
  pass it as template parameter.

  \todo Alternative: store the points as objects!
*/
template <class POINT> class Triangle
{
public:
  static int const myDim = 2;
  static const int numVertices = 3;
  static const int numSides = 3;
  //! Constructs an empty triangle
  /*!
    all points are set to the null pointers
   */
  Triangle() : M_points{nullptr, nullptr, nullptr} {};
  //! Points are given (by reference or pointers)
  /*!\defgroup Constructors Constructors and assignement ops
    @{
  */
  Triangle(POINT &a, POINT &b, POINT &c) : M_points{{&a, &b, &c}} {};
  Triangle(POINT *a, POINT *b, POINT *c) : M_points{{a, b, c}} {};
  //!
  /*!
    If a copy a triangle I copy the pointers to Point
    Beware! It is a shallow copy! To
   */
  Triangle(const Triangle &) = default;
  //! If I assign a triangle i copy the pointers to Poit, beware.
  Triangle &operator=(const Triangle &) = default;
  //! A triangle ca be moved however. No risk here
  Triangle(Triangle &&) = default;
  Triangle &operator=(Triangle &&) = default;
  virtual ~Triangle() = default;
  /*!@}*/
  //! Change/assign a point
  void
  changePoint(int i, POINT &p)
  {
    M_points[i] = &p;
  }
  //! We get the points by operator [] (defined in-class for inlining)
  //! Can be used ONLY if empty()==false
  POINT const &
  operator[](int i) const
  {
    return *(M_points[i]);
  }
  // Can be used ONLY if empty()==false
  POINT &
  operator[](int i)
  {
    return *(M_points[i]);
  }
  //! Triangle area
  double measure() const;
  //! The point on an edge
  POINT const
  edgePoint(int edgenum, int endnum) const
  {
    return *(M_points[M_edge[edgenum][endnum]]);
  }
  // checks if the triangle is empty: M_points contain null pointers
  bool
  empty() const
  {
    return M_points[0] == 0 || M_points[1] == 0 || M_points[2] == 0;
  }
  static unsigned int
  edge(unsigned int edgenum, unsigned int endnum)
  {
    return M_edge[edgenum][endnum];
  }

protected:
  std::array<POINT *, numVertices> M_points;
  static const unsigned int        M_edge[numSides][2];
};

template <class POINT>
const unsigned int Triangle<POINT>::M_edge[Triangle<POINT>::numSides][2] = {
  {0, 1}, {1, 2}, {2, 0}};

template <typename POINT>
double
Triangle<POINT>::measure() const
{
  const Triangle &t = *this;
  return 0.5 * (t[1][0] * (t[2][1] - t[0][1]) + t[2][0] * (t[0][1] - t[1][1]) +
                t[0][0] * (t[1][1] - t[2][1]));
}

} // namespace Geometry
#endif
