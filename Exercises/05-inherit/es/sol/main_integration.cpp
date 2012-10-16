#include <iostream>
#include <cmath>

#include "GetPot"
#include "numerical_integration.hpp"

const double pi=2*std::atan(1.0);

double fexpx ( const double & );

double exact ( const double& a, const double& b );

int main ( int argc, char* argv[] )
{

  using namespace NumericalIntegration;
  using namespace Geometry;

  std::cout.precision(10);

  FunPoint f = fexpx;

  // Select the name for the data file, if specified with argc and argv
  GetPot commandLine ( argc, argv );
  const std::string fileName = commandLine.follow ( "data", 2, "-f", "--file" );

  // Read, using GetPot, the data from file
  GetPot fileData ( fileName.c_str() );

  // Define the master section
  const std::string section = "integration/";

  // Create the domain of integration
  Domain1D domain ( fileData, section + "domain/" );

  // Create the mesh of the domain
  Mesh1D mesh ( domain, fileData, section + "mesh/" );

  std::cout << "Integrale tra " << domain.left()
            << " e " << domain.right()
            << " su " << mesh.numNodes()
            << " intervalli" << std::endl;

  // Create the rule objects
  QuadratureRule* simpRule = new Simpson;
  QuadratureRule* midRule = new MidPoint;
  QuadratureRule* gaussRule = new Gauss4Pt;

  // Create the composite integration objects
  Quadrature s ( *simpRule,  mesh );
  Quadrature m ( *midRule,   mesh );
  Quadrature g ( *gaussRule, mesh );

  // Compute the integrals
  const double approxs = s.apply ( f );
  const double approxm = m.apply ( f );
  const double approxg = g.apply ( f );
  const double exactVal = exact ( domain.left(), domain.right() );

  std::cout << "MidPoint= " << approxm
            <<" Simpson= " << approxs
            << " Gauss= " << approxg
            << " Exact= " << exactVal << std::endl;

  delete simpRule;
  delete midRule;
  delete gaussRule;

  return 0;
}

double fexpx ( const double& x )
{
  return std::exp(x) * x;
}

double exact ( const double& a, const double & b )
{
  return std::exp(b) * ( b - 1.) - std::exp(a) * ( a - 1. );
}
