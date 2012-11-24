#ifndef HH_POLYGON_HH
#define HH_POLYGON_HH
#include <iostream>
#include <vector>
/*
  Questo e` un esempio di gererchia di classi. E` un esempio in realta` vuoto perche i metodi non sono implementati... quindi e` qui solo per scopo didattico!
 */
namespace Geometry
{

  using ::std::cout;
  using ::std::endl;
  using ::std::ostream;
  using ::std::vector;
  
  
  struct Point2D
{
  double x;
  double y;
};

double distance(Point2D const & a, Point2D const & b);

typedef vector<Point2D> Vertices;

class Polygon
{
public:
  explicit Polygon(bool convex=false);
  explicit Polygon(Vertices v, bool convex=false);
  explicit Polygon(Polygon const & p);
  virtual ~Polygon();
  void setVertices(Vertices const & v);
  bool isConvex() const;
  Vertices const & theVertices()const;
  virtual void showMe(ostream & out=cout) const;
  // qui vanno le altre funzionalita`..
  virtual double diameter() const;
  virtual double inscibedCircle() const;
  //
  // Funzioni di trasformazione che operano sui vertici non hanno bisogno
  // di essere definite virtual, perche i vertici sono membri della classe base
  // 
  void translate(double const & x, double const & y);
  void rotate(double const & angle);
  void scale(double const & xfactor, double const & yfactor);
protected:
  bool isconvex;
  Vertices vertexes;
};

class Quadrilateral: public Polygon
{
public:
  Quadrilateral();
  explicit Quadrilateral(Vertices const & v);
  explicit Quadrilateral(Vertices const & v,bool convex);
  explicit Quadrilateral(Quadrilateral const & q);
  //virtual double diameter() const;
  //virtual double inscibedCircle() const;
  virtual ~Quadrilateral();
  virtual void showMe(ostream & out=cout) const;
  // qui vanno le altre funzionalita`..
};

  
class Triangle: public Polygon
{
public:
  Triangle();
  explicit Triangle(Vertices const & v);
  explicit Triangle(Triangle const & q);
  //virtual double diameter() const;
  //virtual double inscibedCircle() const;
  virtual ~Triangle();
  virtual void showMe(ostream & out=cout) const;
  // qui vanno le altre funzionalita`..
};



  class Square: public Quadrilateral
  {
  public:
    Square();
    explicit Square(Vertices const & v);
    explicit Square(Square const & s); 
    //virtual double diameter() const;
    //virtual double inscibedCircle() const;
    void showMe(ostream & out=cout) const;
    // Metodi propri del quadrato
    double diagLen()const;
  };

}

#endif
