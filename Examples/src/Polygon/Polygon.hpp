#ifndef HH_POLYGON_HH
#define HH_POLYGON_HH
#include <iostream>
#include <vector>
#include <array>
#include <utility>

/*!  @file Polygon.hpp
  @brief This is an example of class hierarchy that
  represents poligonal object.

  @detail This module is wrapped in the namespace `Geometry`. It
  represent a first example of class hierarchy with public
  inheritance. The class `AbstractPolygon` defines the general public
  interface of all other classes representing polygonal objects.

  We make use of some syntax of the C++11 new standard so you need to
  compile with *-std=c++11*
   */
namespace Geometry
{

  //! A class that holds 2D points
  /*! It also represents a vector in R2
   */
  class Point2D
  {
  public:
    //! Constructor giving coordinates.
    Point2D(double xx=0.0, double yy=0.0):coor{{xx,yy}}{}
    //! Copy constructor
    Point2D(const Point2D&)=default;
    //! Returns coordinates in a array<double>.
    std::array<double,2> get() const { return coor;}
    //! Sets point coordinates
    void set(double const &xx, double const &yy)
    {
      coor={{xx,yy}};
    }
    //! x coordinate
    double x() const {return coor[0];}
    //! y coordinate
    double y() const {return coor[1];}
    //! Subtraction is implemented as an external friend function
    friend Point2D operator - (Point2D const & a, Point2D const & b);
    //! Addition is implemented as an external friend function
    friend Point2D operator + (Point2D const & a, Point2D const & b);
  private:
    std::array<double,2> coor;
  };

  //! An alias
  using R2Vector=Point2D;

  //! subtraction operator.
  /*!
    It is defined in the header file because I want it to be
    inline.
  */
  inline Point2D operator - (Point2D const & a, Point2D const & b){
    return Point2D(a.coor[0]-b.coor[0],
                   a.coor[1]-b.coor[1]);
  }

  //! Addition operator.
  /*!
    It is defined in the header file because I want it to be
    inline.
  */
  inline Point2D operator + (Point2D const & a, Point2D const & b){
    return Point2D(a.coor[0]+b.coor[0],
                   a.coor[1]+b.coor[1]);
  }

  //! Distance between points
  double distance(Point2D const & a, Point2D const & b);

  //! Polygon vertices are just vectors of points.
  using Vertices=std::vector<Point2D>;

  //! Defines the common interface of polygons.
  class AbstractPolygon
  {
  public:

    virtual ~AbstractPolygon()=default;
    //! Returns the number of vertices.
    /*!  We return Vertices::size_t since it is guaranteed to be the correct
      type for indexes in stl vectors. Its actual type may be implementation
      dependent.
      In this case, however, int would have been fine (size_type is
      guaranteed to be an integral type, more precisely
      a type convertible to unsigned int).
    */
    std::size_t size() const=0;
    //! Is the polygon convex?
    bool isConvex() const {return isconvex;}
    //! Returns the ith vertex
    Point2D vertex(size_t i) const=0
    //! Outputs some info on the polygon
    virtual std::ostream & showMe(std::ostream & out=std::cout) const=0;
    //! The area of the polygon (with sign!).
    /*!
      It is a pure virtual function.
      The implementation is left to the derived classes.
    */
    virtual double area() const=0;
  protected:
    bool isconvex=false;
  };

  //! Class for a generic Polygon
  /*
    A generic Polygon is defined by a set of Vertices which are
    provided by the user
   */
  class Polygon: public AbstractPolygon
  {
  public:
    //! Default constructor.
    //! Polygon may be constructed giving Vertices;
    Polygon(Vertices const & v, bool check=true):vertexes{v}
    {
      if (check) checkConvexity();
    }
    std::size_t size() const override {return vertexes.size();}
    //! Destructor
    virtual ~Polygon(){};
    /*!
      The area is positive if vertices are given in
      counterclockwise order
    */
    virtual double area() const;
    //! Specialised version for generic polygons.
    virtual std::ostream & showMe(std::ostream & out=std::cout) const;
  protected:
    Vertices vertexes;
    void checkConvexity();
  };

  //! A square
  /*!
    The square is a final class derived from polygon.
   */
  class Square final: public AbstractPolygon
  {
  public:
    Square(Vertices const & v);
    //!Special constructor valid only for squares.
    /*!
      /param origin Point which gives the first vertex of the square.
      /param length The length of the side.
      /param angle In radians, tells how the square is  rotated.
     */
    Square(Point2D origin, double length,double angle=0.0);
    Square(Square const &)=default;
    Square(Square&&)=default;
    Square & operator=(const Square &)=default;
    Square & operator=(Square &&)=default;
    //! Specialised version for squares
    double area() const override;
    //! Specialised version for squares.
    std::ostream & showMe(std::ostream & out=std::cout) const override;
  };

  //! A triangle
  class Triangle final: public AbstractPolygon
  {
  public:
    Triangle(Vertices const &);
    Triangle(Triangle const &)=default;
    Triangle(Triangle&&)=default;
    Triangle & operator=(const Triangle &)=default;
    Triangle & operator=(Triangle &&)=default;
    //! Specialised for Triangles
    virtual double area() const override;
    //! Specialised for Triangles
    virtual std::ostream & showMe(std::ostream & out=std::cout) const override;
  };

}

#endif
