#include "typeSwitch.hpp"
int main(){
  Mesh<Triangle> triangleMesh;
  Mesh<Tetra> tetraMesh;
  triangleMesh.boundaryElementList();
  tetraMesh.boundaryElementList();
  Mesh2<2> triangleMesh2;
  // Mesh2<0> wrongMesh2;  //ERROR!
}
