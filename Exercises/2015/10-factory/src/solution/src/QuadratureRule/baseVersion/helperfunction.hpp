#ifndef HELPERFUNCTION_HPP
#define HELPERFUNCTION_HPP
#include <iostream>
//! Helper function to read data from terminal
void readParameters(const int argc, char** argv,
		    double &a, double &b, int & nint,
		    std::ostream & out=std::cout);
#endif
