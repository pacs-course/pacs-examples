#include "shape.hpp"

namespace Geometry {

  std::ostream& operator<<(std::ostream& ostr, const Point& P) {
    ostr << "[" << P.M_ID << "] " << std::flush;
    ostr << "(x = " << P[0]
	 << ", y = " << P[1]
	 << ")" << std::flush;
    return ostr;
  }

  Shape::Shape(int nP, int nE) : M_nP(nP), M_nE(nE), M_points(nP) {}

  Shape::Shape(const Shape& s) : M_nP(s.nP()), M_nE(s.nE()) {
    M_points.resize(M_nP);
    for(int i = 0; i < M_nP; i++)
      M_points[i] = s.point(i);
  }

  std::ostream& operator<<(std::ostream& ostr, const Shape& s) {
    ostr << "[" << s.nP() << "]" << std::endl;
    for(int i = 0; i < s.nP(); i++) {
      ostr << "P" << i << ": " << s.point(i);
      if(i < s.nP() -1)
	ostr << std::endl;
      else
	ostr << std::flush;
    }
    return ostr;
  }

  Edge::Edge(Point P0, Point P1) : 
    Shape(2), 
    M_elemLeft(NOTSET),
    M_elemRight(NOTSET)
  {
    if( P0.id() > P1.id() )
        { 
	  this->point(0) = P0; 
	  this->point(1) = P1; 
	}
      else
	{ 
	  this->point(0) = P1; 
	  this->point(1) = P0; 
	}
    }  

  std::ostream& operator<<(std::ostream& ostr, const Edge& s) {
    ostr << "[" << s.nP() << "]" << std::endl;
    for(int i = 0; i < s.nP(); i++) {
      ostr << "P" << i << ": " << s.point(i);
      if(i < s.nP() -1)
	ostr << std::endl;
      else
	ostr << std::endl;
    }
    ostr << "Left element " << s.elemLeft() << std::endl;
    
    if ( s.elemRight() != NOTSET )
	ostr << "Right element " << s.elemRight() << std::endl;
    else
        ostr << "Right element not in the domain." << std::endl;

    return ostr;
  }

}
