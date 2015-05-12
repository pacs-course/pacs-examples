#include <iostream>
#include <cmath>
#include "GetPot"
#include "numerical_integration.hpp"
#include "numerical_rule.hpp"
#include "helperfunction.hpp"
#include "integrands.hpp"
#include "montecarlo.hpp"
#include "QuadratureRuleAdaptive.hpp"
using namespace NumericalIntegration;
using namespace Geometry;



int main(int argc, char** argv){
  FunPoint f=fsincos;
  
  using namespace std;
  double a,b;
  int nint;
  double targetError;
  readParameters(argc,argv, a, b,nint,targetError);
  cout<<"Integral from "<<a<<" to "<<b <<" on "<< nint <<" intervals"<<endl;
  Domain1D domain(a,b);
  Mesh1D mesh(domain,nint);
  
  Quadrature s(Simpson(),mesh);
  Quadrature m(MidPoint(),mesh);
  Quadrature t(Trapezoidal(),mesh);


  cout<<" Now the mesh has "<<mesh.numNodes()<<" nodes"<<endl;

  double approxs=s.apply(f);
  
  double approxm=m.apply(f);
  double exactVal=exact(a,b);
  cout<<"MidPoint="<<approxm<<" Trapezoidal="<<t.apply(f)<<" Simpson="<<approxs<<" Exact="<<exactVal<<endl;

  // Now with MonteCarlo

  MonteCarlo mcRule;
  // Desired error
  mcRule.setError(1.e-3);
  Quadrature mc(mcRule,mesh);
  
  auto approxmc = mc.apply(f);
  cout<<"MonteCarlo="<<approxmc<<" error="<<
    std::abs(exactVal-approxmc)<<" Estimated/error="<<
    dynamic_cast<MonteCarlo const &>(mc.myRule()).cumulatedError()/
    std::abs(exactVal-approxmc)<<endl;
  
  //Now the adaptive
  Quadrature sa(QuadratureRuleAdaptive<Simpson>(targetError),mesh);
  double adaptiveResult=sa.apply(f);
  printout(adaptiveResult,exactVal,targetError,"SImpson Adaptive");

  
}
  
