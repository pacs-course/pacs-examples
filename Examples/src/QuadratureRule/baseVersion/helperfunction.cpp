#include <cmath>
#include <cstddef>
#include "helperfunction.hpp"
#include "GetPot"
//! Helper function to read data from terminal
namespace apsc::NumericalIntegration
{

  void readParameters(const int argc, char** argv,
		      double &a, double &b, int & nint, double & targetError,
		      std::ostream & out)
  {
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
  
  void readParameters(const int argc, char** argv,
		      double &a, double &b, int & nint, double & targetError,
                      std::string & library,std::string & integrand,
		      std::ostream & out)
  {
    GetPot cl(argc, argv);
    const double pi=2*std::atan(1.0);
    if(cl.search(2, "--help", "-h")){
      out<<"Compute integral from a to b with nint intervals"<<"\n";
      out<<"Possible arguments:"<<"\n";
      out<<"a=Value  (default 0)"<<"\n";
      out<<"b=Value (default pi)"<<"\n";
      out<<"nint=Value (default 10)"<<"\n";
      out<<"targetError=Value (default 1e-4)"<<"\n";
      out<<"library=string (default libintegrands.so)"<<"\n";
      out<<"integrand=string (default fsincos)"<<"\n";
      std::exit(1);
    }
    a=cl("a", 0.);
    b=cl("b", 1*pi);
    nint=cl("nint", 10);
    targetError=cl("targetError",1e-4);
    library=cl("library", "./libintegrands.so");
    integrand=cl("integrand", "fsincos");
  }
  
  void printout(double const value,double const exact,double const error,
                std::string const & title,std::ostream & out)
  {
    out<< "*** "<<title<<" ***"<<std::endl;
    out<< "Computed value "<<value<<std::endl;
    out<< "Error          "<<(exact-value)<<std::endl;
    out<< "Target error   "<<error<<std::endl<<std::endl;
  }
  
}
