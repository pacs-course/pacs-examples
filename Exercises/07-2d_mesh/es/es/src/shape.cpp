#include "shape.hpp"

namespace Geometry {
  std::ostream& operator<<(std::ostream& ostr, const Point& P) {
    ostr << "[" << P.M_ID << "] " << std::flush;
    ostr << "(x = " << P[0]
	 << ", y = " << P[1]
	 << ")" << std::flush;
    return ostr;
  }

  Shape::Shape(int nP) : M_nP(nP), M_nE(0), M_points(nP) {}

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
}
