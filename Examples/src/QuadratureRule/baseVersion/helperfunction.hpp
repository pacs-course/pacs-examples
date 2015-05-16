#ifndef HELPERFUNCTION_HPP
#define HELPERFUNCTION_HPP
#include <iostream>
#include <string>
//! Helper function to read data from terminal
namespace NumericalIntegration
{
  //! Reads parameters given as option
  //! @param argc number of options
  //! @param argv Options string
  //! @param a left boundary of integration interval
  //! @param b right boundary of integration interval
  //! @param nint of integration sub-interval
  //! @param targetError Target error (for adaptive)
  //! @param cout Output stream for the '-h' option
  void readParameters(const int argc, char** argv,
                      double &a, double &b, int & nint,double & targetError,
                      std::ostream & out=std::cout);
  
  // A simple utility to output some results
  void printout(double const value,double const exact,double const error,
                std::string const & title,std::ostream & out=std::cout);
  
  //! Reads parameters given as option
  //! @param argc number of options
  //! @param argv Options string
  //! @param a left boundary of integration interval
  //! @param b right boundary of integration interval
  //! @param nint of integration sub-interval
  //! @param targetError Target error (for adaptive)
  //! @param library Library of integrands
  //! @param integrand the desired integrand
  //! @param cout Output stream for the '-h' option
  void readParameters(const int argc, char** argv,
		      double &a, double &b, int & nint, double & targetError,
                      std::string & library, std::string & integrand,
		      std::ostream & out);

}
#endif
