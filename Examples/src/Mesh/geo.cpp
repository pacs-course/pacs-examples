#include "geo.hpp"

namespace Geometry
{
Point
operator-(const Point &a, const Point &b)
{
  return Point(a.M_coor[0] - b.M_coor[0], a.M_coor[1] - b.M_coor[1]);
}

Point
operator+(const Point &a, const Point &b)
{
  return Point(a.M_coor[0] + b.M_coor[0], a.M_coor[1] + b.M_coor[1]);
}

Point
Point::operator*(const double &d) const
{
  return Point(d * M_coor[0], d * M_coor[1]);
}

Point
operator*(const double &d, const Point &p)
{
  return p * d;
}
} // namespace Geometry
