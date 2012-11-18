#include <cmath>
#include <cstddef>
#include "helperfunction.hpp"
#include "GetPot"
//! Helper function to read data from terminal
void readParameters(const int argc, char** argv,
		    double &a, double &b, int & nint,
		    std::ostream & out){
  GetPot cl(argc, argv);
  const double pi=2*std::atan(1.0);
  if(cl.search(2, "--help", "-h")){
    out<<"Compute integral from a to b with nint intervals"<<"\n";
    out<<"Possible arguments:"<<"\n";
    out<<"a=Value  (default 0)"<<"\n";
    out<<"b=Value (default pi)"<<"\n";
    out<<"nint=Value (default 10)"<<"\n";
    out<<"-h or --help : this help"<<"\n";
    std::exit(1);
  }
  a=cl("a", 0.);
  b=cl("b", 1*pi);
  nint=cl("nint", 10);
}
