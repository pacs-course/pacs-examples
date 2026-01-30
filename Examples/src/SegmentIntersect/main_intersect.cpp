#include "EdgeGeo.hpp"
#include "SegmentIntersect.hpp"
#include <format>

std::ostream &
operator<<(std::ostream &os, apsc::Geometry::EdgeGeo<> const &e)
{
  os << std::format("EdgeGeo: P0=({}, {}) P1=({}, {})", e[0][0], e[0][1],
                    e[1][0], e[1][1]);
  return os;
}

int
main()
{
  using namespace apsc::Geometry;
  using apsc::Geometry::EdgeGeo;
  {
    Point p1 = {0.0, -1.0};
    Point p2 = {0.0, 2.0};
    Point p3 = {0., 2.0};
    Point p4 = {0.0, 0.0};

    EdgeGeo a(p1, p2);
    EdgeGeo b(p3, p4);
    std::cout << std::format("Edge a: \n");
    std::cout << a << std::endl;
    std::cout << std::format("Edge b: \n");
    std::cout << b;
    auto intersection = segmentIntersect(a, b);
    std::cout << intersection;
    std::cout << std::endl << "---------------" << std::endl;
  }

  {
    Point p1 = {-1, -1.0};
    Point p2 = {1., 2.0};
    Point p3 = {0, -2.0};
    Point p4 = {1, 10.0};

    EdgeGeo a(p1, p2);
    EdgeGeo b(p3, p4);
    std::cout << std::format("Edge a: \n");
    std::cout << a << std::endl;
    std::cout << std::format("Edge b: \n");
    std::cout << b << std::endl;
    auto intersection = segmentIntersect(a, b);
    std::cout << intersection;
    std::cout << std::endl << "---------------" << std::endl;
  }

  {
    Point p1 = {-1, -1.0};
    Point p2 = {1., 1.0};
    Point p3 = {1, 2.0};
    Point p4 = {-1, 0.0};

    EdgeGeo a(p1, p2);
    EdgeGeo b(p3, p4);
    std::cout << std::format("Edge a: \n");
    std::cout << a << std::endl;
    std::cout << std::format("Edge b: \n");
    std::cout << b << std::endl;
    auto intersection = segmentIntersect(a, b);
    std::cout << intersection;
    std::cout << std::endl << "---------------" << std::endl;
  }
  return 0;
}
