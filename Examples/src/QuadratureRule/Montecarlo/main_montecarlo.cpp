#include <iostream>
#include <cmath>
#include "GetPot"
#include "numerical_integration.hpp"
#include "numerical_rule.hpp"
#include "helperfunction.hpp"
#include "montecarlo.hpp"
const double pi=2*std::atan(1.0);

double fsincos(double const &);

double fsincos(double const & x)
{
  using namespace std;
  return sin(x)*cos(x);
};

double one(double const & x)
{return 1;}

double exact(double const& a, const double & b)
{
  using namespace std;
  return 0.5*(sin(b)*sin(b)-sin(a)*sin(a));
};

using namespace NumericalIntegration;
using namespace Geometry;



int main(int argc, char** argv){
  FunPoint f=fsincos;
  
  using namespace std;
  double a,b;
  int nint;
  readParameters(argc,argv, a, b,nint);
  cout<<"Integral from "<<a<<" to "<<b <<" on "<< nint <<" intervals"<<endl;
  Domain1D domain(a,b);
  Mesh1D mesh(domain,nint);
  
  Quadrature s(Simpson(),mesh);
  Quadrature m(MidPoint(),mesh);
  Quadrature t(Trapezoidal(),mesh);
  MonteCarlo mcRule;
  mcRule.setError(1.e-3);
  Quadrature mc(mcRule,mesh);
  double approxmc = mc.apply(f);
  double approxs  = s.apply(f);
  double approxm  = m.apply(f);
  double approxt  = t.apply(f);
  double exactVal = exact(a,b);

  cout<<"MidPoint="<<approxm<<" Trapezoidal="<<approxt<<" Simpson="<<approxs<<" Exact="<<exactVal<<endl;
  cout<<"MonteCarlo="<<approxmc<<" error="<<
    std::abs(exactVal-approxmc)<<" Estimated/error="<<
    dynamic_cast<MonteCarlo const &>(mc.myRule()).cumulatedError()/
    std::abs(exactVal-approxmc)<<endl;
}
  
