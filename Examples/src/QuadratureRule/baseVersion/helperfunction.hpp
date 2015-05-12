#ifndef HELPERFUNCTION_HPP
#define HELPERFUNCTION_HPP
#include <iostream>
//! Helper function to read data from terminal
namespace NumericalIntegration
{
void readParameters(const int argc, char** argv,
		    double &a, double &b, int & nint,double & TargetError,
		    std::ostream & out=std::cout);
void printout(double const value,double const exact,double const error,
	      std::string const & title);
}
#endif
