#ifndef HH_POLYGON_HH
#define HH_POLYGON_HH
#include <iostream>
#include <vector>
/*
  Questo e` un esempio di gererchia di classi. E` un esempio in realta` vuoto
  perche i metodi non sono implementati... quindi e` qui solo per scopo
  didattico!
 */
namespace Geometry
{
using ::std::cout;
using ::std::endl;
using ::std::ostream;
using ::std::vector;

//! A simple class for points
struct Point2D
{
  double x;
  double y;
};

double distance(Point2D const &a, Point2D const &b);

typedef vector<Point2D> Vertices;
/*!
 * The base calss for Polygons (just for the example, most methods are not
 * implemented)
 */
class Polygon
{
public:
  explicit Polygon(bool convex = false);
  explicit Polygon(Vertices v, bool convex = false);
  virtual ~Polygon();
  void            setVertices(Vertices const &v);
  bool            isConvex() const;
  Vertices const &theVertices() const;
  virtual void    showMe(ostream &out = cout) const;
  // other functionalities
  virtual double diameter() const;
  virtual double inscibedCircle() const;
  //
  void translate(double const &x, double const &y);
  void rotate(double const &angle);
  void scale(double const &xfactor, double const &yfactor);

protected:
  bool     isconvex;
  Vertices vertexes;
};
} // namespace Geometry

#endif
