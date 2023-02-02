#include "MeshReaders.hpp"
#include "MeshTria.hpp"
#include "femMesh.hpp"
#include <iostream>
#include <vector>
/*
 * Read the README.md fie for the instruction on how to create the files for the test.
 */
int
main()
{
  using namespace std;
  using namespace Fem;
  MeshReadSimple reader;
  reader.setverbose(true);
  MeshTria mesh2("mesh.msh", reader);
  mesh2.checkmesh();
  cout << mesh2 << endl;

  cout << " More info on first triangle" << std::endl;
  cout << mesh2.triangle(0) << std::endl;

  MeshReadTriangle readTriangle(true);

  MeshTria meshT("Test/quad.1", readTriangle);
  meshT.checkmesh();
  cout << meshT << endl;

  MeshTria meshA("Test/A.1", readTriangle);
  meshA.checkmesh();


}
