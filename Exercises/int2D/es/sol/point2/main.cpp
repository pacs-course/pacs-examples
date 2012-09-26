#include <iostream>
#include "int2D.hpp"

using namespace Geometry;

typedef double (*FunPoint) ( const double&, const double & );

double f1 ( const double&, const double& );
double f2 ( const double&, const double& );

int main( int argc, char* argv[] )
{
  // Select the name for the data file, if specified with argc and argv
  std::string fileName("data");
  if ( argc == 2 )
    fileName = argv[1];

  // Read, using GetPot, the data from file
  GetPot cl( fileName.c_str() );

  // Define the master section
  std::string section = "integration/";

  Mesh2D mesh( cl, ( section + "mesh/" ) );

  mesh.generateMesh();

  NumericalIntegration::Quadrature integrale( mesh ); 

  std::cout << "Vale of the integral " << integrale.apply( f2, 5 ) << std::endl;

  return 0;
}

double f1( const double& x, const double& y )
{
  return x*x;
}

double f2( const double& x, const double& y )
{
  return x*y;
}
