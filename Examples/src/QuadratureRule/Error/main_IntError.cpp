#include <iostream>
#include <cmath>
#include <string>
#include "GetPot"
#include "numerical_integration.hpp"
#include "QuadratureRulePlusError.hpp"
#include "numerical_rule.hpp"
#include "helperfunction.hpp"
#include "integrands.hpp"

using namespace NumericalIntegration;
using namespace Geometry;

void printout(double const value,double const exact,double const error,
	      std::string const & title){
  std::cout<< "*** "<<title<<" ***"<<std::endl;
  std::cout<< "Computed value "<<value<<std::endl;
  std::cout<< "Error          "<<(exact-value)<<std::endl;
  std::cout<< "Estimated error"<<error<<std::endl<<std::endl;
}

int main(int argc, char** argv){
  FunPoint f=fsincos;
  using namespace NumericalIntegration;
  using namespace std;
  double a,b;
  int nint;
  readParameters(argc,argv, a, b,nint);
  cout<<"Integral from "<<a<<" to "<<b <<" on "<< nint <<" intervals"<<endl;
  Domain1D domain(a,b);
  Mesh1D mesh(domain,nint);
  Simpson simpsonRule;
  MidPoint midPointRule;
  Trapezoidal trapezoidalRule;

  QuadratureRulePlusError sr(simpsonRule);

  QuadratureRulePlusError mr(midPointRule);
  QuadratureRulePlusError tr(trapezoidalRule);

  Quadrature s(sr,mesh);
  // DOES NOT WORK (TO INVESTIGATE WHY)
  //Quadrature m(QuadratureRulePlusError(MidPoint()),mesh);
  Quadrature m(mr,mesh);
  Quadrature t(tr,std::move(mesh));
  double approxs=s.apply(f);
  double errors=NumericalIntegration::ExtractError::error;
  NumericalIntegration::ExtractError::reset();
  double approxm=m.apply(f);
  double errorm=NumericalIntegration::ExtractError::error;
  NumericalIntegration::ExtractError::reset();
  double approxt=t.apply(f);
  double errort=NumericalIntegration::ExtractError::error;
  NumericalIntegration::ExtractError::reset();
  double exactVal=exact(a,b);
  printout(approxs,exactVal,errors,"SIMPSON");
  printout(approxm,exactVal,errorm,"MIDPOINT");
  printout(approxt,exactVal,errort,"TRAPEZOIDAL");
}
  
