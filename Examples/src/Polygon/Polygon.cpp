#include "Polygon.hpp"
#include <cmath>
#include <cstdlib>
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
  return (u.x() * v.y() - u.y() * v.x()) / (norm(u) * norm(v));
}
// ********************* BASE CLASS **********************

AbstractPolygon::AbstractPolygon(Vertices const &v, bool check) : vertexes{v}
{
  if(check)
    this->checkConvexity();
}

void
AbstractPolygon::setVertexes(Vertices const &v)
{
  vertexes = v;
  checkConvexity();
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
  //! for simplicity
  Vertices const &myV = this->vertexes;
  auto            mysize = this->size();
  // We consider segments and triangles as convex
  if(mysize <= 3)
    {
      this->isconvex = true;
      return true;
    }
  //! Since we are dealing with floating points it is better to have
  //  a small number so that |a| < smallNumber means for us a==0
  //  numeric_limit<dpuble>::epsilon() is a constexp function
  double constexpr smallNumber = std::numeric_limits<double>::epsilon();
  double res{0.0};
  double newres{0.0};
  for(std::size_t i = 0; i < mysize; ++i)
    {
      Point2D p = myV[i];
      //  next point (circulating around)
      Point2D v = myV[(i + 1) % myV.size()] - p;
      // next next point
      Point2D u = myV[(i + 2) % myV.size()] - p;
      if(i == 0)
        // in first loop direction is unknown, so save it in res
        {
          res = sinAngle(u, v);
        }
      else
        {
          newres = sinAngle(u, v);
          if(std::abs(res) < smallNumber)
            {
              // The two edges are aligned, skip test and update res
              res = newres;
            }
          else if(newres * res < 0)
            {
              // angle has changed sign
              return this->isconvex = false;
            }
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
      Point2D const &p0 = vertexes[(i - 1) % siz];
      result += p1.x() * (p2.y() - p0.y());
    }
  return 0.5 * result;
}

std::ostream &
Polygon::showMe(std::ostream &out) const
{
  std::cout << " A Generic Polygon" << std::endl;
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
  double constexpr smallNumber = 100 * std::numeric_limits<double>::epsilon();
  // Check if it is a square!
  double l1 = distance(vertexes[1], vertexes[0]);
  double l2 = distance(vertexes[2], vertexes[3]);
  auto   ratio = std::abs(this->area()) / (l1 * l2);
  if(std::abs(ratio - 1.0) > smallNumber)
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
  ;
}

std::ostream &
Triangle::showMe(std::ostream &out) const
{
  out << "A Triangle" << std::endl;
  return AbstractPolygon::showMe(out);
}

} // namespace Geometry
