#include "Polygon.hpp"
#include <iostream>

namespace Geometry
{
// ********************* BASE CLASS **********************
Polygon::Polygon(bool convex) : isconvex(convex), vertexes() {}
Vertices const &
Polygon::theVertices() const
{
  return vertexes;
};
Polygon::Polygon(Vertices v, bool convex) : isconvex(convex), vertexes(v) {}
void
Polygon::setVertices(Vertices const &v)
{
  vertexes = v;
}
Polygon::~Polygon() {}
bool
Polygon::isConvex() const
{
  return isconvex;
}
void
Polygon::showMe(ostream &out) const
{
  out << "Just a bare Polygon" << std::endl;
}

double
Polygon::inscibedCircle() const
{
  // to be implemented
  return 0;
}
double
Polygon::diameter() const
{
  // to be implemented
  return 0;
}
void
Polygon::translate(double const &x, double const &y)
{
  // to be implemented
}
void
Polygon::rotate(double const &angle)
{
  // to be implemented
}
void
Polygon::scale(double const &xfactor, double const &yfactor)
{
  // to be implemented
}

} // namespace Geometry
