#include <iostream>
#include <cmath>
#include <string>
#include "GetPot"
#include "numerical_integration.hpp"
#include "QuadratureRuleAdaptive.hpp"
#include "numerical_rule.hpp"
#include "integrands.hpp"

using namespace NumericalIntegration;
using namespace Geometry;

//! Helper function to read data from terminal
void readParameters(const int argc, char** argv,
		    double &a, double &b, int & nint,
		    double & targetError,
		    std::ostream & out=std::cout){
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

int main(int argc, char** argv){
  FunPoint f=fsincos;
  using namespace NumericalIntegration;
  using namespace std;
  double a,b,targetError;
  int nint;
  readParameters(argc,argv, a, b,nint,targetError);
  cout<<"Integral from "<<a<<" to "<<b <<" on "<< nint <<" intervals"<<endl;
  double targetPerInt=targetError/static_cast<double>(nint);
  cout<<"Error per interval= "<<targetPerInt<<endl;
  Domain1D domain(a,b);
  Mesh1D mesh(domain,nint);
  Simpson simpsonRule;
  MidPoint midPointRule;
  Trapezoidal trapezoidalRule;

  QuadratureRuleAdaptive sr(simpsonRule,targetPerInt);

  QuadratureRuleAdaptive mr(midPointRule,targetPerInt,1000);
  QuadratureRuleAdaptive tr(trapezoidalRule,targetPerInt,1000);

  Quadrature s(sr,mesh);
  Quadrature m(mr,mesh);
  Quadrature t(tr,std::move(mesh));
  std::cout <<" SIMPSON"<<std::endl;
  double approxs=s.apply(f);
  std::cout <<" MIDPOINT"<<std::endl;
  double approxm=m.apply(f);
  std::cout <<" TRAPEZOIDAL"<<std::endl;
  double approxt=t.apply(f);
  double exactVal=exact(a,b);
  printout(approxs,exactVal,targetError,"SIMPSON");
  printout(approxm,exactVal,targetError,"MIDPOINT");
  printout(approxt,exactVal,targetError,"TRAPEZOIDAL");
}
  
