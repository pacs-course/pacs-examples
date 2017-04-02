#include <iostream>
#include <cmath>
#include "GetPot"
#include "./src/numerical_integration.hpp"
#include "./src/mesh.hpp"
const double pi=2*std::atan(1.0);
using namespace NumericalIntegration;
using namespace Geometry;

// Using functions
double fsincos(double const &);

double fsincos(double const & x)
{
  using namespace std;
  return sin(x)*cos(x);
};
double exact(double const& a, const double & b)
{
  using namespace std;
  return 0.5*(sin(b)*sin(b)-sin(a)*sin(a));
};

double xsinkx(const double& x, const double& k )
{
  return x*std::sin(k*x);
}

double primit( const double& x, const double& k)
{
  return (std::sin(k*x)-k*x*std::cos(k*x))/(k*k);
}

int main(int argc, char* argv[])
{


  FunPoint f=fsincos;
  
  using namespace std;
  cout.precision(10);

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
  Trapezoidal trapRule;
  Gauss4Pt gaussRule;

  Quadrature  s ( mesh, simpRule );
  Quadrature  m ( mesh, midRule );
  Quadrature  t ( mesh, trapRule );
  Quadrature  g ( mesh, gaussRule );

  double approxs = s.apply(f);
  double approxm = m.apply(f);
  double approxt = t.apply(f);
  double approxg = g.apply(f);
  double exactVal= exact(domain.left(), domain.right());

  cout << "MidPoint= " << approxm 
       << " Simpson= " << approxs
       << " Trapezoidal= " << approxt
       << " Gauss= " << approxg
       << " Exact= " << exactVal 
       << endl;

  return 0;  
}
  
