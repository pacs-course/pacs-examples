#include <iostream>
#include "src/mesh.hpp"

int main() {
  using namespace Geometry;

  Mesh m("mesh.msh");
  std::cout << m << std::flush;

  return 0;
}
