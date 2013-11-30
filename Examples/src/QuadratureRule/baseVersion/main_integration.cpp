#include <iostream>
#include <cmath>
#include "GetPot"
#include "numerical_integration.hpp"
#include "numerical_rule.hpp"
#include "helperfunction.hpp"
#include "integrands.hpp"

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
  Quadrature t(Trapezoidal(),std::move(mesh));
  // Now the mesh is empty
  cout<<" Now the mesh has "<<mesh.numNodes()<<" nodes"<<endl;

  double approxs=s.apply(f);
  
  double approxm=m.apply(f);
  double exactVal=exact(a,b);
  cout<<"MidPoint="<<approxm<<" Trapezoidal="<<t.apply(f)<<" Simpson="<<approxs<<" Exact="<<exactVal<<endl;
}
  
