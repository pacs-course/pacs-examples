#include <iostream>
#include <cmath>
#include "GetPot"
#include "numerical_integration.hpp"
using namespace NumericalIntegration;

const double pi = 2*std::atan(1.0);

double fsincos ( const double & );

double exact ( const double& a, const double& b );

int main ( int argc, char* argv[] )
{
  // Increase the precision when printing real numbers
  std::cout.precision(10);

  FunPoint f = fsincos;

  // Create the rule objects
  Simpson simpRule;
  MidPoint midRule;

  // Read, using GetPot, the data from input
  GetPot commandLine ( argc, argv );
  const double a = commandLine ( "a", 0. );
  const double b = commandLine ( "b", 1*pi );
  const unsigned int nint = commandLine ( "nint", 10 );

  std::cout << "Integrale tra " << a << " e " << b
            << " su " << nint << " intervalli"
            << std::endl;

  // Create the composite integration objects
  Quadrature s ( simpRule, a, b, nint );
  Quadrature m ( midRule, a, b, nint );

  // Compute the integrals
  const double approxs = s.apply ( f );
  const double approxm = m.apply ( f );
  const double exactVal = exact ( a, b );

  std::cout << "MidPoint= " << approxm
            << " Simpson= " << approxs
            << " Exact=" << exactVal << std::endl;

  return 0;
}

double fsincos ( const double& x )
{
  return std::sin(x) * std::cos(x);
}

double exact ( const double& a, const double & b )
{
  return 0.5 * ( std::sin(b) * std::sin(b) - std::sin(a) * std::sin(a) );
}
