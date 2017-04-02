#include "mesh.hpp"

namespace Geometry {
  Mesh::Mesh(char* fileName) {
    std::ifstream f(fileName);
    std::string currLine;

    if ( f.is_open() ) {

      // Read the number of points and elements
      std::cout << "[Mesh::Mesh] Reading mesh data" << std::endl;
      /*
        NOTE: if not found, this is an infinite loop
      */
      do
        getline(f, currLine); 
      while (currLine.find("# DATA") == std::string::npos);
      int nP, nE;
      f >> nP >> nE;
      M_points.reserve(nP);
      M_elements.reserve(nE);

      // Read the points
      std::cout << "[Mesh::Mesh] Reading points" << std::endl;
      do
        getline(f, currLine);
      while (currLine.find("# POINTS") == std::string::npos);

      for(int i = 0; i < nP; i++) {
        Point P(i);
        f >> P(0) >> P(1);
        M_points.push_back(P);
      }    

      // Read the elements
      std::cout << "[Mesh::Mesh] Reading elements" << std::endl;
      do
        getline(f, currLine);
      while (currLine.find("# ELEMENTS") == std::string::npos);

      for(int i = 0; i < nE; i++) {
        int type;
        f >> type;
        if(type == 0) {
	  int P1, P2, P3;
	  f >> P1 >> P2 >> P3;
          element_pointer_type E ( new Triangle(M_points[P1], 
	 			  M_points[P2], 
				  M_points[P3]) );

	  M_elements.push_back(E);
        } else {
	  int P1, P2, P3, P4;
	  f >> P1 >> P2 >> P3 >> P4;
	  element_pointer_type E ( new Quadrangle(M_points[P1], 
				    M_points[P2], 
				    M_points[P3], 
				    M_points[P4]) );
	  M_elements.push_back(E);
        }
      }
    }
    f.close();
  }

  std::ostream& operator<<(std::ostream& ostr, const Mesh& m) {
    int nP = m.M_points.size();
    int nE = m.M_elements.size();
    // Print mesh points
    ostr << "[operator<<(Mesh)] Points [" 
	 << nP << "]"
	 << std::endl;
    for(int i = 0; i < nP; i++)
      std::cout << m.M_points[i] << std::endl;
    // Print elements
    ostr << "[operator<<(Mesh)] Elements [" 
	 << nE << "]"
	 << std::endl;
    for(int i = 0; i < nE; i++)
      std::cout << *(m.M_elements[i]) << std::endl;
    return ostr;
  }
}
