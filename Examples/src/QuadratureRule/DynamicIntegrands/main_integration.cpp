#include <iostream>
#include <cmath>
#include <string>
#include <dlfcn.h>
#include "GetPot"
#include "numerical_integration.hpp"
#include "numerical_rule.hpp"
#include "helperfunction.hpp"


int main(int argc, char** argv){

  using namespace NumericalIntegration;
  using namespace Geometry;
  using namespace std;
  
  double a,b;
  int nint;
  double targetError;
  std::string library;
  std::string integrand;
  readParameters(argc,argv, a, b,nint,targetError,library,integrand,cout);
  cout<<"Integral from "<<a<<" to "<<b <<" on "<< nint <<" intervals"<<endl;
  cout<<"for function: "<<integrand<<endl;
  Domain1D domain(a,b);
  Mesh1D mesh(domain,nint);
  
  Quadrature s(Simpson(),mesh);
  Quadrature m(MidPoint(),mesh);
  Quadrature t(Trapezoidal(),mesh);
  
  void * lib_handle;
  // Get integrand library
  lib_handle=dlopen(library.c_str(),RTLD_LAZY);
  if (lib_handle==nullptr){
    std::cerr<<"Error in opening library "<<dlerror()<<std::endl;
    std::exit(1);
  }

  
  double (*fp) (double const &); // pointer to function
  // Read integrand from library
  void * ip=dlsym(lib_handle,integrand.c_str());

  char* error=dlerror();
  if(ip==nullptr ||error != 0 ){
    std::cerr<<"Error, invalid integrand "<< error<<std::endl;
    std::exit(2);
  }
  //! interpret pointer as pointer to function
  fp  = reinterpret_cast<double (*) (double const &)>(ip);
  //  FunPoint f(fp);  
  double approxs=s.apply(fp);
  double approxm=m.apply(fp);
  cout<<"MidPoint="<<approxm<<" Trapezoidal="<<t.apply(fp)<<" Simpson="<<approxs<<endl;;
}
  
