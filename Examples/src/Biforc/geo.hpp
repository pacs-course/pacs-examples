#ifndef __GEO_HH__
#define __GEO_HH__
#include <iosfwd>
#include "Eigen/Dense"
namespace Geometry{
  typedef Eigen::Vector2d Vector;
  const int ndim=2;
  class Point
  {
  public:
    static int const myDim=0;
    explicit  Point(double x=0, double y=0);// builds a Point
    Point(const Point &);
    Point & operator=(const Point&);
    void setCoordinates(double x, double y); // Sets point coordinates
    void getCoordinates(double & x, double & y) const; //Get points coordinates
    // We can get the coordinates also by operator []
    double const operator[](int i)const {return M_coor[i];}
    double & operator[](int i) {return M_coor[i];}
    double const x()const {return M_coor[0];}
    double const y()const {return M_coor[1];}
    Point operator +=(const Point &);
    Point operator -=(const Point &);
    friend Point operator +(const Point&, const Point &);
    friend Point operator -(const Point&, const Point &);
    friend std::ostream & operator <<(std::ostream &, Point const &);
    Point operator *(const double &)const;
    //! Dot product
    double dot(Point const &) const ;
    friend Point operator*(const double &, const Point &);
    //! Return point as an eigen vector
    Vector asVector()const{return Vector(M_coor[0],M_coor[1]);}
  private:
    double M_coor[ndim];
  };

  //! Represents a triangle
  /*!
    For a positive triangle points must be clockwise oriented.
   */class Triangle{
  public:
    static int const myDim=2;
    static const int numVertices=3;
    static const int numSides=3;
    Triangle(); //Constructs an empty triangle
    Triangle(Point&,Point&,Point&); //Points are given (by reference)
    Triangle(const Triangle&);
    Triangle & operator=(const Triangle&);
    // We get the points by operator [] (defined in-class for inlining)
    void setPoint(int i, Point const &  p);
    // Can be used ONLY if empty()==false
    Point const & operator[](int i) const {return M_points[i];}
    // Can be used ONLY if empty()==false
    Point & operator[](int i){return M_points[i];}
    double measure() const; // Triangle area
    Point& edgePoint(int edgenum,int endnum); // The point on an edge
    Point const & edgePoint(int edgenum,int endnum) const; // The const version
    //!Get baricenter
    Point baricenter()const;
    //!Get edge baricenter
    Point edgeBaricenter(int edgeNum) const;
    //!Get vector connecting edge baricenter with baricenter
    Vector c(int edgeNum) const;
    // Outward normal to edge times edge length;
    Vector unscaledNormal(int edgeNum)const;
    static int edge(int edgeNum, int endNum); // The edge numbering
    friend std::ostream & operator <<(std::ostream &, Triangle const &);
  private:
    Point  M_points[numVertices];
    static int const M_edge[numSides][2];
  };



} // end namespace Geometry
#endif
