#include "../VisitorAndVariant/geo.hpp"

#include <algorithm>

#include "../VisitorAndVariant/ShapeVisitor.hpp"
namespace Geometry2
{
void
Point::setCoordinates(double x, double y)
{
  M_coor[0] = x;
  M_coor[1] = y;
}


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

const int Triangle::M_edge[3][2] = {{0, 1}, {1, 2}, {2, 0}};

Point
Triangle::changePoint(int i, Point const &p)
{
  Point tmp = M_points[i];
  M_points[i] = p;
  return tmp;
}

Point &
Triangle::edgePoint(int edgenum, int endnum)
{
  return M_points[M_edge[edgenum][endnum]];
}

const Point &
Triangle::edgePoint(int edgenum, int endnum) const
{
  return M_points[M_edge[edgenum][endnum]];
}

int
Triangle::edge(int i, int j)
{
  return M_edge[i][j];
}

double
Triangle::measure() const
{
  const Triangle &t = *this;
  return 0.5 * (t[1][0] * (t[2][1] - t[0][1]) + t[2][0] * (t[0][1] - t[1][1]) +
                t[0][0] * (t[1][1] - t[2][1]));
}


const int Quadrilateral::M_edge[4][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}};

Point
Quadrilateral::changePoint(int i, Point const &p)
{
  Point tmp = M_points[i];
  M_points[i] = p;
  return tmp;
}

Point &
Quadrilateral::edgePoint(int edgenum, int endnum)
{
  return M_points[M_edge[edgenum][endnum]];
}

const Point &
Quadrilateral::edgePoint(int edgenum, int endnum) const
{
  return M_points[M_edge[edgenum][endnum]];
}

int
Quadrilateral::edge(int i, int j)
{
  return M_edge[i][j];
}

double
Quadrilateral::measure() const
{
  const Quadrilateral &t = *this;
  return 0.5 * ((t[1][0] - t[0][0]) * (t[3][1] - t[0][1]) -
                (t[1][1] - t[0][1]) * (t[3][0] - t[0][0]) +
                (t[1][1] - t[2][1]) * (t[3][0] - t[2][0]) -
                (t[1][0] - t[2][0]) * (t[3][1] - t[2][1]));
}
} // namespace Geometry2
