#include <iostream>
#include <cmath>
#include "GetPot"
#include "numerical_integration.hpp"
using namespace NumericalIntegration;
using namespace Geometry;

const double pi=2*std::atan(1.0);

double fexpx( const double & );

double exact( const double& a, const double& b );

int main(int argc, char* argv[])
{
  using namespace std;

  cout.precision(10);

  FunPoint f=fexpx;

  // Select the name for the data file, if specified with argc and argv
  string fileName("data");
  if ( argc == 2 )
    fileName = argv[1];

  // Read, using GetPot, the data from file
  GetPot cl( fileName.c_str() );

  // Define the master section
  string section = "integration/";

  // Create the domain of integration
  Domain1D domain ( cl, section + "domain/" );

  // Create the mesh of the domain
  Mesh1D mesh ( domain, cl, section + "mesh/" );

  cout << "Integrale tra " << domain.left() << " e " << domain.right()
       << " su " << mesh.numNodes() << " intervalli" << endl;

  // Create the rule objects
  Simpson  simpRule;
  MidPoint midRule;
  Gauss4Pt gaussRule;


  // Create the composite integration objects
  Quadrature s ( simpRule,  mesh );
  Quadrature m ( midRule,   mesh );
  Quadrature g ( gaussRule, mesh );

  // Compute the integrals
  double approxs = s.apply(f);
  double approxm = m.apply(f);
  double approxg = g.apply(f);
  double exactVal = exact(domain.left(), domain.right());

  cout << "MidPoint= " << approxm
       <<" Simpson= " << approxs
       << " Gauss= " << approxg
       << " Exact= " << exactVal << endl;
}

double fexpx( const double& x )
{
  using namespace std;
  return exp(x)*x;
};


double exact( const double& a, const double & b )
{
  using namespace std;
  return exp(b)*( b - 1.) - exp(a)*( a - 1 );
};
