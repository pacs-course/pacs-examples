#include "mesh.hpp"

#define MAX_LINES 100

namespace Geometry {

  Mesh::Mesh(char* fileName) {
    std::ifstream f(fileName);
    std::string currLine;

    if ( f.is_open() ) {

      // Read the number of points and elements
      std::cout << "[Mesh::Mesh] Reading mesh data" << std::endl;
      
      // Scan file lines
      int count(0);
      do
        { getline(f, currLine);
        ++count; }
      while( (currLine.find("# DATA") == std::string::npos) && (count < MAX_LINES) );

      // If the string is not found, abort
      if( !(count<MAX_LINES) )
      	{ std::cout << "[Mesh::Mesh] FILE ERROR! max number of lines exceeded"
      		<< " while looking for string '# DATA'" << std::endl;
      		abort(); }

      // Read number of points and elements
      int nP, nE;
      f >> nP >> nE;
      M_points.reserve(nP);
      M_elements.reserve(nE);
      
      // Read the points' coordinates
      std::cout << "[Mesh::Mesh] Reading points" << std::endl;
      count = 0;
      do
        { getline(f, currLine);
        ++count; }
      while( (currLine.find("# POINTS") == std::string::npos) && (count < MAX_LINES) );

      // If the string is not found, abort
      if( !(count<MAX_LINES) )
      	{ std::cout << "[Mesh::Mesh] FILE ERROR! max number of lines exceeded"
      		<< " while looking for string '# POINTS'" << std::endl;
      		abort(); }

      // Fill point data structures
      for(int i = 0; i < nP; i++) {
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
        { std::cout << count << "\t" << std::endl; getline(f, currLine);
        ++count; std::cout << count << "\t" << std::endl;}
      while( (currLine.find("# ELEMENTS") == std::string::npos)  && (count < MAX_LINES) );

      // If the string is not found, abort
      if( !(count<MAX_LINES) )
      	{ std::cout << "[Mesh::Mesh] FILE ERROR! max number of lines exceeded"
      		<< " while looking for string '# ELEMENTS'" << std::endl;
      		abort(); }

      // Fill element data structures
      for(int i = 0; i < nE; i++) {
      	int type;
        f >> type;
        if(type == 0) {
        	int label[3];
        	f >> label[0] >> label[1] >> label[2];
          element_pointer_type E ( new Triangle(M_points[label[0]], 
                                                M_points[label[1]], 
                                                M_points[label[2]]) );

          M_elements.push_back(E);
          for( int j = 0; j < 3; ++j )
          	M_patches.insert( std::make_pair( label[j], E ) );

        } else {
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

  void
  Mesh::buildEdges()
  {
  	// Loop on elements
  	for( element_vector_type::iterator it = M_elements.begin(); it != M_elements.end(); ++it )
  		{
  		// for each element, loop on edges
  		for( int i = 0; i < (*it)->nE(); ++i )
  			{
  				M_edges.insert( (*it)->edge(i) );

  				/*
  				   How to build the boundary edge set:
  				   - if an edge is already present in the set, erase it
  				   - if an edge is not present, insert it
  				   This way only edges which are addressed once will be present in the set
  				 */
  				if( M_boundary_edges.find( (*it)->edge(i) ) != M_boundary_edges.end() )
  					M_boundary_edges.erase( (*it)->edge(i) );
  				else
  					M_boundary_edges.insert( (*it)->edge(i) );
  			} 			  		
   		}

  	std::cout << "[Mesh::buildEdges()] now boundary edges!"
			<< std::endl;

  	/*
  	   How to classify edges
  	   - if an edge has only Neumann nodes, it's a Neumann edge
  	   - if an edge has at least one Dirichlet node, it's not a Neumann edge
  	   We proceed by erasing non-Neumann edges
  	 */
  	M_neumann_boundary_edges = M_boundary_edges;
  	// Loop on edges
  	for( edge_set_type::iterator it = M_neumann_boundary_edges.begin();
  		it != M_neumann_boundary_edges.end(); ++it )
  		{
  		// for each edge, query on points
  		switch( M_map_boundary_string_values[ M_boundary_points[it->point(0).id()] ] )
  			{
  				case Neumann:
  					if( M_boundary_points[it->point(1).id()] == "Dirichlet" )
  						M_neumann_boundary_edges.erase( it );
  					break;
  				case Dirichlet:
  					M_neumann_boundary_edges.erase( it );
  					break;
  				default:
  					break;
  			}
  		}
  }
  
  std::ostream& operator<<(std::ostream& ostr, const Mesh& m) {
  	int nP = m.M_points.size();
    int nE = m.M_elements.size();
    // Print mesh points
    ostr << "[operator<<(Mesh)] Points [" 
    		<< nP << "]"
    		<< std::endl;
    for(int i = 0; i < nP; i++)
    	ostr << m.M_points[i] << std::endl;

    // Print elements
    ostr << "[operator<<(Mesh)] Elements [" 
    		<< nE << "]"
    		<< std::endl;
    for(int i = 0; i < nE; i++)
      ostr << *(m.M_elements[i]) << std::endl;

    // Print patches
    ostr << "[operator<<(Mesh)] Patches [" 
    		<< nP << "]"
    		<< std::endl;
  	typedef Mesh::patch_multimap_type::const_iterator IT;
    for(int i = 0; i < nP; i++)
      { ostr << "[node " << i << "]" << std::endl;
      	std::pair<IT, IT> p_pair = m.M_patches.equal_range( i );
      	while( p_pair.first != p_pair.second )
      	{
      		std::cout << *(*(p_pair.first)).second << std::endl;
      		++p_pair.first;
      	}
      }

    // Print edges
    ostr << "[operator<<(Mesh)] Edges [" 
    		<< m.M_edges.size() << "]"
    		<< std::endl;
    for(Mesh::edge_set_type::iterator it = m.M_edges.begin(); it != m.M_edges.end(); ++it )
      ostr << *it << std::endl;

    // Print boundary edges
    ostr << "[operator<<(Mesh)] Boundary Edges [" 
    		<< m.M_boundary_edges.size() << "]"
    		<< std::endl;
    for(Mesh::edge_set_type::iterator it = m.M_boundary_edges.begin(); it != m.M_boundary_edges.end(); ++it )
      ostr << *it << std::endl;

    return ostr;
  }


  // Print out points having a given boundary condition
  void Mesh::bc( std::string const& bcname )
  {
    typedef boundary_point_map_type::iterator IT;

    IT p = find_if( M_boundary_points.begin(), M_boundary_points.end(),
		    value_equals<int,std::string>( bcname ) );

    if( p == M_boundary_points.end() )
      std::cout << "\nThere are no points with selected BC!" << std::endl;
    else
      {
    		std::cout << "[bc(Mesh)] Points having " 
    		<< bcname << " boundary conditions:\n"
    		<< std::endl;
      }

    while( p != M_boundary_points.end() )
      {
    	print( *p );
    	IT q = find_if( ++p, M_boundary_points.end(),
    	                value_equals<int,std::string>( bcname ) );
    	p = q;
      }

  }

}

void print( std::pair<int,std::string> pr )
{
  std::cout << "\t" << pr.first << "\n" << std::endl;
}

