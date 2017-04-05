#include "mesh.hpp"

#define MAX_LINES 100

namespace Geometry {
  Mesh::Mesh(const char* fileName) {
    std::ifstream f(fileName);
    std::string currLine;

    if ( f.is_open() ) {

      // Read the number of points and elements
      std::cout << "[Mesh::Mesh] Reading mesh data" << std::endl;
      
      // Scan file lines
      int count(0);
      do
        { 
	  getline(f, currLine);
	  ++count; 
	}
      while( (currLine.find("# DATA") == std::string::npos) && (count < MAX_LINES) );

      // If the string is not found, abort
      if( !(count<MAX_LINES) )
      	{ 
	  std::cout << "[Mesh::Mesh] FILE ERROR! max number of lines exceeded"
		    << " while looking for string '# DATA'" << std::endl;
	  abort(); 
	}

      // Read number of points and elements
      int nP, nE;
      f >> nP >> nE;
      M_points.reserve(nP);
      M_elements.reserve(nE);
      
      // Read the points' coordinates
      std::cout << "[Mesh::Mesh] Reading points" << std::endl;
      count = 0;
      do
        { 
	  getline(f, currLine);
	  ++count; 
	}
      while( (currLine.find("# POINTS") == std::string::npos) && (count < MAX_LINES) );

      // If the string is not found, abort
      if( !(count<MAX_LINES) )
      	{ 
	  std::cout << "[Mesh::Mesh] FILE ERROR! max number of lines exceeded"
		    << " while looking for string '# POINTS'" << std::endl;
	  abort(); 
	}

      // Fill point data structures
      for(int i = 0; i < nP; i++) 
	{
	  Point P(i);
	  std::string bc;
	  f >> P(0) >> P(1) >> bc;
	  M_points.push_back(P);
	  M_boundary_points.insert( make_pair( i, bc ) );
	  M_boundary_conditions.insert( make_pair( bc, i ) );
	}    

      // this is used when M_patches is a vector of lists
      // M_patches.resize(nP);

      // Read the elements
      std::cout << "[Mesh::Mesh] Reading elements" << std::endl;
      count = 0;
      do
        { 
	  std::cout << count << "\t" << std::endl; 
	  getline(f, currLine);
	  ++count; 
	  std::cout << count << "\t" << std::endl;
	}
      while( (currLine.find("# ELEMENTS") == std::string::npos)  && (count < MAX_LINES) );

      // If the string is not found, abort
      if( !(count<MAX_LINES) )
      	{ 
	  std::cout << "[Mesh::Mesh] FILE ERROR! max number of lines exceeded"
		    << " while looking for string '# ELEMENTS'" << std::endl;
	  abort(); 
	}

      // Fill element data structures
      for(int i = 0; i < nE; i++) 
	{
      	int type;
        f >> type;
        if(type == 0) 
	  {
	    int label[3];
	    f >> label[0] >> label[1] >> label[2];
	    element_pointer_type E ( new Triangle(M_points[label[0]], 
						  M_points[label[1]], 
						  M_points[label[2]]) );

	    M_elements.push_back(E);
	    for( int j = 0; j < 3; ++j )
	      M_patches.insert( std::make_pair( label[j], E ) );

	  } 
	else 
	  {
	    int label[4];
	    f >> label[0] >> label[1] >> label[2] >> label[3];
	    element_pointer_type E ( new Quadrangle(M_points[label[0]], 
						    M_points[label[1]], 
						    M_points[label[2]], 
						    M_points[label[3]]) );
	    M_elements.push_back(E);
	    for( int j = 0; j < 4; ++j )
	      M_patches.insert( std::make_pair( label[j], E ));

	    // this is used when M_patches is a vector of lists of element_pointer_type
	    // M_patches[i].push_back(E);
	  }
	}
    }
    f.close();
    
    M_map_boundary_string_values["Dirichlet"] = Dirichlet;
    M_map_boundary_string_values["Neumann"] = Neumann;
    M_map_boundary_string_values["Internal"] = Internal;
  
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
