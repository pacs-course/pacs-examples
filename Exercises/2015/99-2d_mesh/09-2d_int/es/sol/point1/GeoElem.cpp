#include "GeoElem.hpp"

namespace Geometry
{

  // --------- Point ---------
  
  Point::Point( const double& x, const double& y ):
    M_coor(2)
  {
    M_coor[0]=x; 
    M_coor[1]=y;
  }

  Point operator -(const Point & a, const Point & b)
  {
    return Point(a.M_coor[0]-b.M_coor[0],
		 a.M_coor[1]-b.M_coor[1]);
  }

  Point operator +(const Point & a, const Point & b)
  {
    return Point(a.M_coor[0]+b.M_coor[0],
		 a.M_coor[1]+b.M_coor[1]);
  }
  
  Point Point::operator *(const double & d) const
  {
    return Point(d*M_coor[0],d*M_coor[1]);
    }

  Point::Point(const Point & p):
    M_coor( ndim )
  {
    M_coor[0]=p.M_coor[0];
    M_coor[1]=p.M_coor[1];
  }
  
  Point & Point::operator =(const Point & p)
  {
    if(this!=&p)
      {
	M_coor[0]=p.M_coor[0];
	M_coor[1]=p.M_coor[1];
      }
    return *this;
  }
  
  Point operator *(const double & d, const Point & p)
  {
    return p*d;
  }

  // ---------- Triangle -----------

  Triangle::Triangle():
    M_points( numVertices )
  {
    M_points[0] = M_points[1] = M_points[2] = Point();
  }

  Triangle::Triangle( const Point & a, 
		      const Point & b, 
		      const Point & c ):
    M_points( numVertices )
  {
    M_points[0] = a;
    M_points[1] = b;
    M_points[2] = c;
  }

  Triangle::Triangle( const Triangle & t ):
    M_points( numVertices )
  {
    M_points[0] = t.M_points[0];
    M_points[1] = t.M_points[1];
    M_points[2] = t.M_points[2];
  }

  Triangle& Triangle::operator =(const Triangle & t)
  {
    if(this!=&t)
      {
	M_points[0] = t.M_points[0];
	M_points[1] = t.M_points[1];
	M_points[2] = t.M_points[2];
      }
    return *this;
  }
  
}
