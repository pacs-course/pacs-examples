#include <cmath>
#include <cstddef>
#include "helperfunction.hpp"
#include "GetPot"
//! Helper function to read data from terminal
namespace NumericalIntegration
{
void readParameters(const int argc, char** argv,
		    double &a, double &b, int & nint, double & targetError,
		    std::ostream & out){
  GetPot cl(argc, argv);
  const double pi=2*std::atan(1.0);
  if(cl.search(2, "--help", "-h")){
    out<<"Compute integral from a to b with nint intervals"<<"\n";
    out<<"Possible arguments:"<<"\n";
    out<<"a=Value  (default 0)"<<"\n";
    out<<"b=Value (default pi)"<<"\n";
    out<<"nint=Value (default 10)"<<"\n";
    out<<"targetError=Value (default 1e-4)"<<"\n";
    out<<"-h or --help : this help"<<"\n";
    std::exit(1);
  }
  a=cl("a", 0.);
  b=cl("b", 1*pi);
  nint=cl("nint", 10);
  targetError=cl("targetError",1e-4);
}

void printout(double const value,double const exact,double const error,
	      std::string const & title){
  std::cout<< "*** "<<title<<" ***"<<std::endl;
  std::cout<< "Computed value "<<value<<std::endl;
  std::cout<< "Error          "<<(exact-value)<<std::endl;
  std::cout<< "Target error   "<<error<<std::endl<<std::endl;
}
}
