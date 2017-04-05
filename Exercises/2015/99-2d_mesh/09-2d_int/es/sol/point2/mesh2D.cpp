#include "mesh2D.hpp"
#include <sstream>
#include <fstream>

namespace Geometry
{

  // Constructor.
  Mesh2D::Mesh2D ( const GetPot&      data,
		   const std::string& section ):
    M_data( data ),
    M_section( section )
  { } // Constructor.

  // Virtual destructor.
  Mesh2D::~Mesh2D ()
  {} // Virtual destructor.

  void Mesh2D::generateMesh()
  {
    std::ostringstream os;
    const std::string fileName = M_data( ( M_section + "name" ).data(), "box" );
    
    
    os << M_data( ( M_section + "triangle_path" ).data(), "./" )
       <<"triangle " << M_data( ( M_section + "flag" ).data(), "-cq30a.03")
       << " " << fileName << ".poly";

    // Call Triangle to generate the mesh
    system( os.str().c_str() );

    std::vector<Point> points;
    unsigned int useless;
    unsigned int nPoints;

    std::ifstream filePoint;
    filePoint.open ( ( fileName + ".1.node").data(), std::ifstream::in);

    if ( filePoint.is_open() )
      {

	filePoint >> nPoints;

	points.reserve( nPoints );

	// Read useless data
	filePoint >> useless;
	filePoint >> useless;
	filePoint >> useless;	

	for ( unsigned int i = 0; i < nPoints; ++i )
	  {
	    double x, y;
	    filePoint >> useless;
	    filePoint >> x;
	    filePoint >> y;
	    filePoint >> useless;
	    points.push_back( Point(x, y) );
	  }
    
      }
    else
      {
	std::cerr << "Miss file" << std::endl;
	abort();
      }

    filePoint.close();

    std::ifstream fileElem;
    unsigned int nElem;
    fileElem.open ( ( fileName + ".1.ele").data(), std::ifstream::in);

    if ( fileElem.is_open() )
      {

	fileElem >> nElem;

	M_triangles.reserve( nElem );

	// Read useless data
	fileElem >> useless;
	fileElem >> useless;

	for ( unsigned int i = 0; i < nElem; ++i )
	  {
	    int ID1, ID2, ID3;
	    fileElem >> useless;
	    fileElem >> ID1;
	    fileElem >> ID2;
	    fileElem >> ID3;

	    M_triangles.push_back( Triangle(points[ID1 - 1], points[ID2 - 1], points[ID3 - 1]) );
	    
	  }
    
      }
    else
      {
	std::cerr << "Miss file" << std::endl;
	abort();
      }
    
    fileElem.close();

  }

}
