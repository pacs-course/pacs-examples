#ifndef __GEO_HH_
#define __GEO_HH_
#include <array>
namespace Geometry{
  const int ndim=2;
  class ShapeVisitor;// FORWARD DECLARATION

  class Shape{
  public:
    Shape()=default;
    virtual void accept(ShapeVisitor &)=0;
    virtual ~Shape()=default;
    virtual double measure() const;
  };

  class Point : public Shape
  {
  public:
    static int const myDim=0;
    explicit  Point(double x=0, double y=0);// builds a Point
    Point(const Point &);
    Point & operator=(const Point&);
    virtual ~Point(){};
    void setCoordinates(double x, double y); // Sets point coordinates
    void getCoordinates(double & x, double & y) const; //Get points coordinates
    // We can get the coordinates also by operator []
    double const operator[](int i)const {return M_coor[i];}
    double & operator[](int i) {return M_coor[i];}
    friend Point operator +(const Point&, const Point &);
    friend Point operator -(const Point&, const Point &);
    Point operator *(const double &)const;
    friend Point operator*(const double &, const Point &);
    // I need to initialize the constant in-class because
    // we use it to dimension an array member
    void accept(ShapeVisitor &) override;
  private:
    double M_coor[ndim];
  };


  class Triangle: public Shape{
  public:
    static int const myDim=2;
    static const int numVertices=3;
    static const int numSides=3;
    Triangle(); //Constructs an empty triangle
    Triangle(Point&,Point&,Point&); //Points are given (by reference)
    Triangle(const Triangle&);
    Triangle & operator=(const Triangle&);
    // We get the points by operator [] (defined in-class for inlining)
    Point * changePoint(int i, Point &p);
    // Can be used ONLY if empty()==false
    Point const & operator[](int i) const {return *(M_points[i]);}
    // Can be used ONLY if empty()==false
    Point & operator[](int i){return *(M_points[i]);}
    std::array<Point*,numVertices>& getPoints(){return M_points;}
    double measure() const override; // Triangle area
    Point& edgePoint(int edgenum,int endnum); // The point on an edge
    Point const & edgePoint(int edgenum,int endnum) const; // The const version
    //checks if the triangle is empty: M_points contain null pointers
    bool empty()const {return
    		M_points[0]==0||M_points[1]==0||M_points[2]==0;}// inlined function
    static int edge(int edgenum, int endnum); // The edge numbering
    void accept(ShapeVisitor&) override;
  private:
    std::array<Point*,numVertices>  M_points;
    static int const M_edge[numSides][2];
  };
  
  class Square: public Shape{
  public:
    static int const myDim=2;
    static const int numVertices=4;
    static const int numSides=4;
    Square(); //Constructs an empty triangle
    Square(Point&,Point&,Point&,Point&); //Points are given (by reference)
    Square(const Square&);
    Square & operator=(const Square&);
    // We get the points by operator [] (defined in-class for inlining)
    Point * changePoint(int i, Point &p);
    // Can be used ONLY if empty()==false
    Point const & operator[](int i) const {return *(M_points[i]);}
    // Can be used ONLY if empty()==false
    Point & operator[](int i){return *(M_points[i]);}
    std::array<Point*,numVertices>& getPoints(){return M_points;}
    double measure() const override; // area
    Point& edgePoint(int edgenum,int endnum); // The point on an edge
    Point const & edgePoint(int edgenum,int endnum) const; // The const version
    //checks if the triangle is empty: M_points contain null pointers
    bool empty()const {return
        M_points[0]==0||M_points[1]==0||
        M_points[2]==0||M_points[3]==0;}// inline function
    static int edge(int edgenum, int endnum); // The edge numbering
    void accept(ShapeVisitor&) override;
  private:
    std::array<Point*,numVertices>  M_points;
    static int const M_edge[numSides][2];
  };
  }
#endif
