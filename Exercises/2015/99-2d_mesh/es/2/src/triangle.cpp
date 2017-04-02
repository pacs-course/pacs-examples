#include "triangle.hpp"

namespace Geometry {
  Edge Triangle::edge(int i) {
    const int connectivity[3][2] = {{0, 1}, {1, 2}, {2, 0}};
    return Edge(this->point(connectivity[i][0]),
		this->point(connectivity[i][1]));
  };
}
