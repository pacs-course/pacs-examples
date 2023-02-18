#include "Polygon.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
namespace Geometry
{
double
distance(Point2D const &a, Point2D const &b)
{
  // Not very efficient. This function should be implemented either
  // as friend or as a method, to access private members.
  Point2D tmp(a - b);
  return std::sqrt(tmp.x() * tmp.x() + tmp.y() * tmp.y());
}
// ********************* BASE CLASS **********************

void
AbstractPolygon::showMe(std::ostream &out) const
{
  if(this->size() == 0)
    {
      out << "empty polygon" << std::endl;
    }
  else
    {
      out << "Vertices:" << std::endl;
      out << "\tX\t\tY" << std::endl;
      for(auto const &i : this->vertexes)
        out << "\t"<<i.x() << "\t\t" << i.y() << std::endl;
    }
  out << " Area =" << this->area() << std::endl;
  if(this->isconvex)
    out << " Polygon is convex" << std::endl;
  else
    out << " Polygon is not convex" << std::endl;
}

void
AbstractPolygon::checkConvexity()
{
  Vertices const &myV(this->vertexes);
  auto            mysize = this->size();
  // We consider segments and triangles as convex
  if(mysize <= 3)
    {
      this->isconvex = true;
      return;
    }
  //! Since we are dealing with floating points it is better to have
  //  a small number so that |a| < smallNumber means for us a==0
  double const smallNumber(1000 * std::numeric_limits<double>::min());
  Point2D      p;
  Point2D      v;
  Point2D      u;
  double       res(0.0);
  double       newres(0.0);
  //! C++11 sintax. decltype(expr) returns the type of the expression
  for(decltype(mysize) i = 0; i < mysize; ++i)
    {
      p = myV[i];
      // ! next point
      Point2D tmp = myV[(i + 1) % myV.size()];
      v = tmp - p;
      //! next next point
      u = myV[(i + 2) % myV.size()];
      if(i == 0) // in first loop direction is unknown, so save it in res
        res = u.x() * v.y() - u.y() * v.x() + v.x() * p.y() - v.y() * p.x();
      else
        {
          newres =
            u.x() * v.y() - u.y() * v.x() + v.x() * p.y() - v.y() * p.x();
          if(std::abs(res) < smallNumber)
            {
              // The two edges are aligned, skip test and update res
              res = newres;
            }
          else if((newres > 0 && res < 0) || (newres < 0 && res > 0))
            {
              this->isconvex = false;
              return;
            }
        }
    } // end for
  this->isconvex = true;
  return;
}

void
Polygon::set(Vertices const &v)
{
  this->vertexes = v;
  this->checkConvexity();
}

//! To compute the area of a polygon we use the divergence theorem.
/*!
  \f$ int_P d\Omega = 1/2 \int{\partial P} xn_x d\gamma\f$
  The integral is computed by using trapezoidal rule on the polygon sides.
*/
double
Polygon::area() const
{
  auto siz = this->size();
  if(siz < 3)
    return 0;
  double          result(0);
  Vertices const &Ver(this->vertexes);
  // I am using C++11 sintax. decltype(expr) returns the
  // type of the expression expr. (I could have used Vertices::size_type,
  // or just int, here I wanted to show some features of the new standard).
  for(std::size_t i = 0; i < siz; ++i)
    {
      // Current point I use references to avoid unnecessary construction
      // of objects
      Point2D const &p1(Ver[i]);
      // Other point
      Point2D const &p2(Ver[(i + 1) % siz]);
      Point2D const &p0(Ver[(i - 1) % siz]);
      result += p1.x() * (p2.y() - p0.y());
    }
  return 0.5 * result;
}

void
Polygon::showMe(std::ostream &out) const
{
  out << " A generic polygon" << std::endl;
  AbstractPolygon::showMe(out);
}

// ********************* SQUARE **********************
Square::Square(Point2D const &origin, double length, double angle)
{
  this->isconvex = true;
  this->vertexes.reserve(4);
  vertexes.push_back(origin);
  double c(std::cos(angle));
  double s(std::sin(angle));
  vertexes.push_back(Point2D(origin.x() + length * c, origin.y() + length * s));
  vertexes.push_back(
    Point2D(origin.x() + length * (c - s), origin.y() + length * (c + s)));
  vertexes.push_back(Point2D(origin.x() - length * s, origin.y() + length * c));
}

double
Square::area() const
{
  if(this->size() == 0)
    return 0.0;
  // I want the area with sign, positive if the quad is
  // oriented counterclockwise. So the easiest thing is to use the
  // formula using cross product (even if it is not the most efficient choice)
  Point2D v(this->vertexes[1] - this->vertexes[0]);
  Point2D w(this->vertexes[2] - this->vertexes[0]);
  // area = v \times w. Positive if square counterclockwise oriented
  return v.x() * w.y() - v.y() * w.x();
  ;
}

void
Square::showMe(std::ostream &out) const
{
  out << "A Square" << std::endl;
  AbstractPolygon::showMe(out);
}

// ********************* TRIANGOLO **********************

Triangle::Triangle(Vertices const &v) : AbstractPolygon(v)
{
  // Check if we give 3 vertices
  // We may use assert, in this case we would disable the control
  // in the released version (-DNDEBUG). We prefer here to exit the program
  if(this->size() != 3)
    {
      std::cerr << " A trianlge must be created giving three vertices"
                << std::endl;
      std::exit(1);
    }
}

double
Triangle::area() const
{
  if(this->size() == 0)
    return 0.0;
  // I use the cross product since this is a
  // signed area!
  Point2D v(this->vertexes[1] - this->vertexes[0]);
  Point2D w(this->vertexes[2] - this->vertexes[0]);
  // area = 0.5* v \times w. Positive if triangle counterclockwise oriented
  return 0.5 * (v.x() * w.y() - v.y() * w.x());
  ;
}

void
Triangle::showMe(std::ostream &out) const
{
  out << "A Triangle" << std::endl;
  AbstractPolygon::showMe(out);
}
} // namespace Geometry
