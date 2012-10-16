#include<iostream>
#include<vector>
#include "femMesh.hpp"
#include "MeshTria.hpp"
#include "MeshReaders.hpp"
int main()
{
  using namespace std;
  using namespace Fem;
  MeshReadSimple reader;
  reader.setverbose(true);
  MeshTria mesh2(reader);
  mesh2.readMesh(std::string("mesh.msh"));
  mesh2.checkmesh();
  cout<<mesh2<<endl;

  cout<<" More info on first triangle"<<std::endl;
  cout<<mesh2.triangle(0)<<std::endl;
}
