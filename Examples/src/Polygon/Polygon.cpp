#include "Polygon.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
namespace Geometry
{
std::ostream &
operator<<(std::ostream &out, Point2D const &a)
{
  out << "(" << a.coor[0] << ",\t " << a.coor[1] << ")\n";
  return out;
}

double
distance(Point2D const &a, Point2D const &b)
{
  return norm(a - b);
}

double
norm(Point2D const &a)
{
  // Not very efficient. This function should be implemented either
  // as friend or as a method, to access private members.
  return std::sqrt(a.x() * a.x() + a.y() * a.y());
}

// Not very efficient. This function should be implemented either
// as friend or as a method, to access private members.
double
sinAngle(Point2D const &u, Point2D const &v)
{
  auto const nu = norm(u);
  auto const nv = norm(v);
  if(nu <= std::numeric_limits<double>::epsilon() ||
     nv <= std::numeric_limits<double>::epsilon())
    return 0.0;
  return (u.x() * v.y() - u.y() * v.x()) / (nu * nv);
}
// ********************* BASE CLASS **********************

AbstractPolygon::AbstractPolygon(Vertices const &v, bool check) : vertexes{v}
{
  if(check)
    this->isconvex = this->checkConvexity();
}

void
AbstractPolygon::setVertexes(Vertices const &v)
{
  vertexes = v;
  isconvex = checkConvexity();
}

std::ostream &
AbstractPolygon::showMe(std::ostream &out) const
{
  if(this->size() == 0)
    {
      out << "empty polygon" << std::endl;
    }
  else
    {
      out << "Vertices:" << std::endl;
      out << "X\t"
          << " Y" << std::endl;
      for(auto const &i : this->vertexes)
        out << i;
    }
  if(this->isconvex)
    out << " Polygon is convex" << std::endl;
  else
    out << " Polygon is not convex" << std::endl;
  return out;
}

bool
AbstractPolygon::checkConvexity()
{
  Vertices const &myV = this->vertexes;
  auto const      mysize = this->size();
  // We consider segments and triangles as convex
  if(mysize <= 3)
    {
      this->isconvex = true;
      return true;
    }
  //! Since we are dealing with floating points it is better to have
  //  a small number so that |a| < smallNumber means for us a==0
  //  std::numeric_limits<double>::epsilon() is a constexpr function
  double constexpr smallNumber = std::numeric_limits<double>::epsilon();
  double           previousSign{0.0};
  for(std::size_t i = 0; i < mysize; ++i)
    {
      auto const &p0 = myV[i];
      auto const &p1 = myV[(i + 1) % mysize];
      auto const &p2 = myV[(i + 2) % mysize];
      auto const  e1 = p1 - p0;
      auto const  e2 = p2 - p1;
      auto const  cross = e1.x() * e2.y() - e1.y() * e2.x();
      if(std::abs(cross) < smallNumber)
        // in first loop direction is unknown, so save it in res
        {
          continue;
        }
      auto const sign = (cross > 0.0) ? 1.0 : -1.0;
      if(previousSign == 0.0)
        {
          previousSign = sign;
        }
      else if(sign * previousSign < 0.0)
        {
          return this->isconvex = false;
        }
    } // end for
  return this->isconvex = true;
}

// ****   POLYGON

//! To compute the area of a genericpolygon we use the divergence theorem.
/*!
  \f$ int_P d\Omega = 1/2 \int{\partial P} x n_x d\gamma\f$
  The integral is computed by using trapezoidal rule on the polygon sides.
*/
double
Polygon::area() const
{
  auto siz = this->size();
  if(siz < 3)
    return 0.0;
  double result{0};
  for(std::size_t i = 0; i < siz; ++i)
    {
      // Current point I use references to avoid unnecessary construction
      // of objects
      Point2D const &p1 = vertexes[i];
      // Other point
      Point2D const &p2 = vertexes[(i + 1) % siz];
      Point2D const &p0 = vertexes[(i + siz - 1) % siz];
      result += p1.x() * (p2.y() - p0.y());
    }
  return 0.5 * result;
}

std::ostream &
Polygon::showMe(std::ostream &out) const
{
  out << " A Generic Polygon" << std::endl;
  return AbstractPolygon::showMe(out);
}

// ********************* SQUARE **********************

void
Square::checkSquare()
{
  if(vertexes.size() != 4)
    {
      throw std::runtime_error(
        " A square must be created giving four vertices");
    }
  double constexpr tol = 100.0 * std::numeric_limits<double>::epsilon();
  auto const       e0 = vertexes[1] - vertexes[0];
  auto const       e1 = vertexes[2] - vertexes[1];
  auto const       e2 = vertexes[3] - vertexes[2];
  auto const       e3 = vertexes[0] - vertexes[3];
  auto const sqLen = [](Point2D const &v)
  { return v.x() * v.x() + v.y() * v.y(); };
  auto const dot = [](Point2D const &a, Point2D const &b)
  { return a.x() * b.x() + a.y() * b.y(); };

  auto const l0 = sqLen(e0);
  auto const l1 = sqLen(e1);
  auto const l2 = sqLen(e2);
  auto const l3 = sqLen(e3);

  if(l0 <= tol || l1 <= tol || l2 <= tol || l3 <= tol)
    {
      throw std::runtime_error("Vertexes define a degenerate square");
    }

  auto const sameLen = [tol](double a, double b)
  { return std::abs(a - b) <= tol * std::max({1.0, std::abs(a), std::abs(b)}); };

  auto const orthogonal = [tol, &dot](Point2D const &a, Point2D const &b)
  { return std::abs(dot(a, b)) <= tol * std::max({1.0, norm(a) * norm(b)}); };

  if(!(sameLen(l0, l1) && sameLen(l1, l2) && sameLen(l2, l3) &&
       orthogonal(e0, e1) && orthogonal(e1, e2) && orthogonal(e2, e3) &&
       orthogonal(e3, e0)))
    {
      throw std::runtime_error("Vertexes do not define a square");
    }
}

Square::Square(Vertices const &v) : AbstractPolygon{v, false}
{
  this->isconvex = true;
  this->checkSquare();
}

Square::Square(Point2D origin, double length, double angle)
{
  this->isconvex = true;
  this->vertexes.reserve(4);
  vertexes.push_back(origin);
  double c{std::cos(angle)};
  double s{std::sin(angle)};
  vertexes.push_back(Point2D{origin.x() + length * c, origin.y() + length * s});
  vertexes.push_back(
    Point2D{origin.x() + length * (c - s), origin.y() + length * (c + s)});
  vertexes.push_back(Point2D{origin.x() - length * s, origin.y() + length * c});
}

void
Square::setVertexes(Vertices const &v)
{
  this->vertexes = v;
  checkSquare();
}

double
Square::area() const
{
  if(this->size() == 0)
    return 0.0;
  // I want the area with sign, positive if the quad is
  // oriented counterclockwise. So the easiest thing is to use the
  // formula using cross product (even if it is not the most efficient choice)
  Point2D v{this->vertexes[1] - this->vertexes[0]};
  Point2D w{this->vertexes[2] - this->vertexes[0]};
  // area = v \times w. Positive if square counterclockwise oriented
  return v.x() * w.y() - v.y() * w.x();
}

std::ostream &
Square::showMe(std::ostream &out) const
{
  out << "A Square" << std::endl;
  return AbstractPolygon::showMe(out);
}

//********************* TRIANGLE **********************

Triangle::Triangle(Vertices const &v) : AbstractPolygon(v, false)
{
  this->isconvex = true;
  checkTriangle();
}

void
Triangle::setVertexes(Vertices const &v)
{
  this->vertexes = v;
  checkTriangle();
}

void
Triangle::checkTriangle()
{
  if(this->size() != 3)
    {
      throw std::runtime_error(
        " A triangle must be created giving three vertices");
    }
}

double
Triangle::area() const
{
  if(this->size() == 0)
    return 0.0;
  // I use the cross product since this is a
  // signed area!
  Point2D v{this->vertexes[1] - this->vertexes[0]};
  Point2D w{this->vertexes[2] - this->vertexes[0]};
  // area = 0.5* v \times w. Positive if triangle counterclockwise oriented
  return 0.5 * (v.x() * w.y() - v.y() * w.x());
}

std::ostream &
Triangle::showMe(std::ostream &out) const
{
  out << "A Triangle" << std::endl;
  return AbstractPolygon::showMe(out);
}

} // namespace Geometry
