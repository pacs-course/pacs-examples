#include "../SwitchType/typeSwitch.hpp"
//! This is only an example. A real situation will be more complete
//!
//! But you should understand the technique
int
main()
{
  using namespace apsc::Geometry2;
  Mesh<2> triangleMesh;
  using TriangleBoundaryType = Mesh<2>::BoundaryType;
  triangleMesh.addBElement(TriangleBoundaryType{});
  triangleMesh.addBElement(TriangleBoundaryType{});
  triangleMesh.printBoundaries(std::cout);
  Mesh<3> tetraMesh;
  using TetraBoundaryType = Mesh<3>::BoundaryType;
  tetraMesh.addBElement(TetraBoundaryType{});
  tetraMesh.addBElement(TetraBoundaryType{});
  tetraMesh.printBoundaries(std::cout);
}
