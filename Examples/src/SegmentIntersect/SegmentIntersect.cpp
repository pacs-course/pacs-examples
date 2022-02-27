#include "SegmentIntersect.hpp"
#include <iomanip>
#include <iostream>

std::ostream &
operator<<(std::ostream &out,
           apsc::Geometry::Intersection::IntersectionStatus const &i) {
  out << "*Segment intersections:" << std::endl;
  out << "\tSegments intersect    :" << std::boolalpha << i.intersect
      << std::endl;
  if (i.intersect) {
    out << "\tNumber of intersections:" << i.numberOfIntersections << std::endl;
    for (auto j = 0u; j < i.numberOfIntersections; ++j) {
      out << "\tx[" << j << "]=" << std::left << std::setw(10)
          << i.intersectionPoint[j][0];
      out << " y[" << j << "]=" << std::left << std::setw(10)
          << i.intersectionPoint[j][1];
      out << "\n";
      out << "\tt0  =" << std::left << std::setw(10)
          << i.intersection_tcoor[j][0] << " t1  =" << std::left << std::setw(10)
          << i.intersection_tcoor[j][1] << std::endl;
    }
    out << "\tSegments are identical:" << std::boolalpha << i.identical
        << std::endl;
  }
  out << "\tSegments are parallel :" << std::boolalpha << i.parallel
      << std::endl;
  out << "\tSegments are collinear:" << std::boolalpha << i.collinear
      << std::endl;
  if (i.intersect) {
    for (unsigned int j = 0u; j < 2; ++j) {
      for (unsigned int k = 0u; k < 2; ++k) {
        if (i.endPointIsIntersection[j][k]) {
          out << "\tEndPoint " << k << " of segment " << j
              << " is an intersection" << std::endl;
          if (i.otherEdgePoint[j][k] != -1)
            out << "\tand is joined to EdgePoint " << i.otherEdgePoint[j][k]
                << " of segment " << (j + 1) % 2 << std::endl;
        }
      }
    }
  }
  return out;
}
