#include "quadrangle.hpp"

namespace Geometry {
  Edge Quadrangle::edge(const int& i) {
    const int connectivity[4][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}};
    return Edge(this->point(connectivity[i][0]),
		this->point(connectivity[i][1]));
  };
}
