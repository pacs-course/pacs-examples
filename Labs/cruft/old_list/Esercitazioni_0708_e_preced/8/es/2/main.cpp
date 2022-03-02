/*!
  \file main.cpp
  \brief Compilable file for exercise 8, point 2
  \author Tiziano Passerini
  \date 12/15/2006
*/

#include <iostream>
#include <cmath>
#include <string>

#include "src/fpnewton.hpp"
#include "src/fpsolve.hpp"

using namespace FixedPoint;


double f( const double& x )
{
  return std::pow( std::cos(2*x), 2 ) - std::pow(x,2);
}


double df( const double& x )
{
  return -4*std::cos(2*x)*std::sin(2*x)-2*x;
}


int main(){

  Newton d(f,df);

  SolveFP<Newton> newtonsolver( 1e-10, 100, d );
  ResType result = newtonsolver.iterate( 0.01 );

  std::string converged( (result.status)? ("converged.") : ("did not converge.") );
  std::cout << "\nNewton Solver (Fixed Point) for Root Finding:"
	    << "\n\tzero = " << result.val
	    << "\n\tnit = " << result.nit
	    << "\nThe method " << converged
	    << std::endl;

  return 0;
}
