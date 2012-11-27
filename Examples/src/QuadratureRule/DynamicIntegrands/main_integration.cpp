#include <iostream>
#include <cmath>
#include <string>
#include <dlfcn.h>
#include "GetPot"
#include "numerical_integration.hpp"
#include "numerical_rule.hpp"
#include "helperfunction.hpp"

void readParameters(const int argc, char** argv,
		    double &a, double &b, int & nint,
		    std::string & library,std::string & integrand,
		    std::ostream & out){
  GetPot cl(argc, argv);
  const double pi=2*std::atan(1.0);
  if(cl.search(2, "--help", "-h")){
    out<<"Compute integral from a to b with nint intervals"<<"\n";
    out<<"Possible arguments:"<<"\n";
    out<<"a=Value  (default 0)"<<"\n";
    out<<"b=Value (default pi)"<<"\n";
    out<<"nint=Value (default 10)"<<"\n";
    out<<"library=string (default libintegrands.so)"<<"\n";
    out<<"integrand=string (default fsincos)"<<"\n";
    out<<"-h or --help : this help"<<"\n";
    std::exit(1);
  }
  a=cl("a", 0.);
  b=cl("b", 1*pi);
  nint=cl("nint", 10);
  library=cl("library", "./libintegrands.so");
  integrand=cl("integrand", "fsincos");
}



int main(int argc, char** argv){

  using namespace NumericalIntegration;
  using namespace Geometry;
  using namespace std;
  
  double a,b;
  int nint;
  std::string library;
  std::string integrand;
  readParameters(argc,argv, a, b,nint,library,integrand,cout);
  cout<<"Integral from "<<a<<" to "<<b <<" on "<< nint <<" intervals"<<endl;
  cout<<"for function: "<<integrand<<endl;
  Domain1D domain(a,b);
  Mesh1D mesh(domain,nint);
  
  Quadrature s(Simpson(),mesh);
  Quadrature m(MidPoint(),mesh);
  Quadrature t(Trapezoidal(),mesh);
  // Now the mesh is empty
  cout<<" Now the mesh has "<<mesh.numNodes()<<" nodes"<<endl;
  
  void * lib_handle;
  lib_handle=dlopen(library.c_str(),RTLD_LAZY);
  if (!lib_handle){
    std::cerr<<"Error in opening library "<<dlerror()<<std::endl;
    std::exit(1);
  }

  
  double (*fp) (double const &); // pointer to function
  
  void * ip=dlsym(lib_handle,integrand.c_str());

  char* error;
  if(ip==0 || (error = dlerror()) != 0 ){
    std::cerr<<"Error, invalid integrand "<< error<<std::endl;
    std::exit(2);
  }

  fp  = reinterpret_cast<double (*) (double const &)>(ip);
  FunPoint f(fp);  
  double approxs=s.apply(f);
  double approxm=m.apply(f);
  cout<<"MidPoint="<<approxm<<" Trapezoidal="<<t.apply(f)<<" Simpson="<<approxs<<endl;;
}
  
