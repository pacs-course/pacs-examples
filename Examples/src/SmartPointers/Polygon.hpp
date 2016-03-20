#ifndef HH_POLYGON_HH
#define HH_POLYGON_HH
#include <iostream>
#include <vector>
#include <utility>

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
  
  using std::cout;
  using std::endl;
  using std::ostream;
  using std::vector;
  using std::pair;
  

  //! A class that holds points
  class Point2D
  {
  public:
    //! Constructor giving coordinates.
    Point2D(double xx=0.0, double yy=0.0):coor{xx,yy}{} //C++11 syntax
    //! Constructor with a pair.
    Point2D(pair<double,double> c):coor{c} {}
    //! Returns coordinates in a pair<double>.
    std::pair<double,double> get() const { return coor;}
    //! Sets point coordinates
    void set(double const &xx, double const &yy){
      coor=std::make_pair(xx,yy);}
    //! x coordinate
    double x() const {return coor.first;}
    //! y coordinate
    double y() const {return coor.second;}
    //! Subtraction is implemented as an external friend function
    friend Point2D operator - (Point2D const & a, Point2D const & b);
    //! Addition is implemented as an external friend function
    friend Point2D operator + (Point2D const & a, Point2D const & b);
  private:
    std::pair<double,double> coor;
  };

  //! subtraction operator.
  /*!  
    It is defined in the header file because I want it to be
    inline.
  */
  inline Point2D operator - (Point2D const & a, Point2D const & b){
    return Point2D(a.coor.first-b.coor.first,
                   a.coor.second-b.coor.second);
  }

  //! Addition operator.
  /*!  
    It is defined in the header file because I want it to be
    inline.
  */
  inline Point2D operator + (Point2D const & a, Point2D const & b){
    return Point2D(a.coor.first+b.coor.first,
                   a.coor.second+b.coor.second);
  }

  //! Distance between points
  double distance(Point2D const & a, Point2D const & b);
 
  //! Polygon vertices are just vectors of points.
  typedef vector<Point2D> Vertices;

  //! Defines the common interface of polygons.
  class AbstractPolygon
  {
  public:
    //! Empty polygon
    AbstractPolygon():vertexes(),isconvex(false){};
    //! virtual destructor (is a base class)
    virtual ~AbstractPolygon()=default;
    //! Returns the number of vertices.
    /*!  We return Vertices::size_type and not just int because
      size_type is guaranteed to be the correct type for indexes in
      stl vectors. Its actual type may be implementation dependent.
      In this case, however, int would have been fine (size_type is
      guaranteed to be an integral type, more precisely
      a type convertible to unsigned int).
    */
    Vertices::size_type size() const {return vertexes.size();}
    //! Is the polygon convex?
    bool isConvex() const {return isconvex;}
    //! Returns the vertices (read only)
    Vertices const & theVertices()const {return vertexes;}
    //! Outputs some info on the polygon
    virtual void showMe(ostream & out=cout) const;
    //! The area of the polygon (with sign!).
    /*!
      It is a pure virtual function.
      The implementation is left to the derived classes.
    */
    virtual double area() const=0;
  protected:
    //! Protected constructor taking vertices.
    /* 
       This constructor is kept protected because it is not part of
       the interface of AbstractPolygon, yet some derived classes may
       make use of it
    */
    AbstractPolygon(Vertices const & v);
    Vertices vertexes;
    bool isconvex;
    //! Test convexity of the polygon
    void checkConvexity();
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
    /*!
      It does nothing.
     */
    Polygon()=default;// C++11
    //! Polygon may be constructed giving Vertices;
    Polygon(Vertices const &);
    /*!
      The area is positive if vertices are given in 
      counterclockwise order
     */
    virtual double area() const;
    //! Set vertices for a Polygon.
    void set(Vertices const & v);
    //! Specialised version for generic polygons.
    virtual void showMe(ostream & out=cout) const;
  };

  //! A square
  /*!
    The square is a final class derived from polygon.
    The final keyword has been introduced in C++11 
   */
  class Square final : public AbstractPolygon
  {
  public:
    Square(){this->isconvex=true;};
    //!Special constructor valid only for squares.
    /*!
      /param origin Point which gives the first vertex of the square.
      /param length The length of the side.
      /param angle In radians, tells how the square is  rotated. 
     */
    Square(Point2D origin, double length,double angle=0.0);
    //! Specialised version for squares
    virtual double area() const;
    //! Strange destructor to show what happens if you uses smart ptrs
    ~Square(){std::cout<<"Destroying a square"<<std::endl;};
    //! Specialised version for squares.
    virtual void showMe(ostream & out=cout) const;
  };
  
  //! A triangle
  class Triangle final : public AbstractPolygon
  {
  public:
    Triangle(){this->isconvex=true;};
    //!Constructor for triangles.
    /*!
      It uses the general copy contructor but it verifies if
      we give 3 points, if not the program exits with exit(1)
      It may be an interesting case to implement exceptions
     */
    Triangle(Vertices const &);
    //! Specialised for Triangles
    virtual double area() const;
    //
    //! Strange destructor to show what happens if you uses smart ptrs
    ~Triangle(){std::cout<<"Destroying a Triangle"<<std::endl;};
    //! Specialised for Triangles
    virtual void showMe(ostream & out=cout) const;
  };
  
}

#endif
