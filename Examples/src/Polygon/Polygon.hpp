#ifndef HH_POLYGON_HH
#define HH_POLYGON_HH
#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <memory>

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
  /*! It can also represents a vector in R2
   */
  class Point2D
  {
  public:
    //! Constructor giving coordinates.
    Point2D(double xx=0.0, double yy=0.0):coor{xx,yy}{}
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
    //! I implement the streaming operator as a friend
    friend std::ostream& operator << (std::ostream &, Point2D const & b);    
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

  //! Point2D may be used also ad "vector" in 2D so it makes sense to compute
  //! the norm, which is the distance from the origin
  double norm(Point2D const & a);
  //! Return the sin of the angle between two Point2d interpreted as vectors.
  double sinAngle (Point2D const & u, Point2D const & v );
  //! Polygon vertices are just vectors of points.
  using Vertices=std::vector<Point2D>;

  //! Defines the common interface of polygons.
  class AbstractPolygon
  {
  public:
    //! Constructor taking n. of vertices
    /*!
      It produces a Polygon with all points set to zero
    */
    explicit AbstractPolygon(std::size_t numVertices):
      vertexes(numVertices, Point2D{0.,0.}){};
    /*!
      I can also give the vertexes directly
      as a vector of Point2D
      @param v The verices
      @param checkConvex if true verify if the polygon is convex
    */
    AbstractPolygon(Vertices const & v, bool checkConvex=true);
    //! Default constructor is defaulted
    /*!
      It is up to the derived classes to fill the vertices
      correctly
    */
    AbstractPolygon()=default;
    //! virtual destructor
    virtual ~AbstractPolygon()=default;
    //! Returns the number of vertices.
    /*!  We return std::size_t and not just int because
      size_t is guaranteed to be the correct type for indexes in
      stl vectors. Its actual type may be implementation dependent.
      In this case, however, int would have been fine (size_type is
      guaranteed to be an unsigned integral type).
    */
    virtual std::size_t size() const {return vertexes.size();}
    //! Is the polygon convex?
    /*!
      If you are changing vertices you need first to run checkConvexity()
    */
    bool isConvex() const {return isconvex;}
    //! Returns a vertex (read only)
    Point2D vertex(std::size_t i)const {return vertexes[i];}
    //! Returns a vertex (read and write)
    Point2D & vertex(std::size_t i){return vertexes[i];}
    //! I can give the vertexes
    virtual void setVertexes(Vertices const & v);
    //! This allows to iterate on vertices like a vector
    //! It returns an iterator
    auto  begin(){return vertexes.begin();}
    //! This allows to iterate on vertices like a vector
    auto  end(){return vertexes.end();}
    //! This allows to iterate on vertices like a vector
    auto const cbegin() const {return vertexes.cbegin();}
    //! This allows to iterate on vertices like a vector
    auto const cend() const {return vertexes.cend();}
    //! Test convexity of the polygon
    virtual bool checkConvexity();
    //! Outputs some info on the polygon
    virtual std::ostream & showMe(std::ostream & out=std::cout) const;
    //! The area of the polygon (with sign!).
    /*!
      It is a pure virtual function.
      The implementation is left to the derived classes.
    */
    virtual double area() const=0;
  protected:
    //! make it clonable
    virtual std::unique_ptr<AbstractPolygon> clone()=0;
    Vertices vertexes;
    bool isconvex=false;
  };

  //! Class for a generic Polygon
  /*
    A generic Polygon is defined by a set of Vertices which are
    provided by the user
   */
  class Polygon final: public AbstractPolygon
  {
  public:
    //! I inherit all constuctors of AbstractPolygon (C++17)
    using AbstractPolygon::AbstractPolygon;
    //! Destructor
    ~Polygon(){};
    /*!
      The area is positive if vertices are given in
      counterclockwise order
    */
    double area() const override;
    //! Specialised version for generic polygons.
    std::ostream & showMe(std::ostream & out=std::cout) const override;
  protected:
    //! make it clonable
    std::unique_ptr<AbstractPolygon> clone() override{
      return std::make_unique<Polygon>(*this);
    }
  };

  //! A square
  /*!
    The square is a final class derived from AbstractPolygon.
   */
  class Square final: public AbstractPolygon
  {
  public:
    // A square is always convex. And the number of vertices is 4
    Square(Vertices const & v);
    //
    Square(): AbstractPolygon{4}{isconvex=true;}
    //!Special constructor valid only for squares.
    /*!
      /param origin Point which gives the first vertex of the square.
      /param length The length of the side.
      /param angle In radians, tells how the square is  rotated.
     */
    Square(Point2D origin, double length,double angle=0.0);
    // Specialised version for square
    std::size_t size() const override {return nVertices;}
    //! specialised version for Square
    bool checkConvexity() override {return isconvex=true;}
    //! Specialised version for squares
    double area() const override;
    //! Specialised version for squares
    void setVertexes(Vertices const & v) override;
    //! Specialised version for squares.
    std::ostream & showMe(std::ostream & out=std::cout) const override;
    //! Just to show the use of a static constexpr
    static size_t constexpr nVertices=4;
  protected:
    //! make it clonable
    std::unique_ptr<AbstractPolygon> clone() override{
      return std::make_unique<Square>(*this);
    }
  private:
    //! It throws an exception if it is not a square
    void checkSquare();
  };

  //! A triangle
  class Triangle final: public AbstractPolygon
  {
  public:
    Triangle(Vertices const &);
    //! Specialised for Triangles
    Triangle():AbstractPolygon{3}{isconvex=true;}
    // Specialised version for triangle
    std::size_t size() const override {return nVertices;}
    //! specialised version for Triangle
    bool checkConvexity() override{ return isconvex=true;};
    double area() const override;//! Specialised for Triangles
    //! Specialised for Triangles
    void setVertexes(Vertices const & v) override;
    //! Specialised for Triangles
    std::ostream & showMe(std::ostream & out=std::cout) const override;
    //! Just to show the use of a static constexpr
    static size_t constexpr nVertices=3;
  protected:
    //! make it clonable
    std::unique_ptr<AbstractPolygon> clone() override{
      return std::make_unique<Triangle>(*this);
    }

  private:
    //! Throws if not a triangle
    void checkTriangle();
  };

}

#endif
