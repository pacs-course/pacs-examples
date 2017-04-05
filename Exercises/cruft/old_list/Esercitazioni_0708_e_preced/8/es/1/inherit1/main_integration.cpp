/*!
  \file main_integration.cpp
  \brief Compilable file for exercise 8, point 1
  \author Luca Formaggia
*/

#include <iostream>
#include <cmath>

#include "GetPot"

#include "src/numerical_integration.hpp"

using namespace NumericalIntegration;


double farctan(double const & x)
{
  using namespace std;
  return atan(10*x);
};


double exact(double const& a, const double & b)
{
  using namespace std;
  return b*atan(10*b) - (log(1+100*pow(b,2))/20)
    - a*atan(10*a) + (log(1+100*pow(a,2))/20);
};


int main(int argc, char** argv){

  FunPoint f=farctan;

  using namespace std;

  /* 
     Creates a GetPot variable, with an internal
     database of names read from command line
  */
  GetPot   cl(argc, argv);
  double a=cl("a", -3.);
  double b=cl("b", 4);
  int nint=cl("nint", 10);

  cout << "Integrale tra " << a << " e " << b
       << " su " << nint << " intervalli" <<endl;

  QuadratureBase * s=new Simpson(a,b,nint);
  MidPoint  m(a,b,nint);
  QuadratureBase & mr(m);

  double approxs=s->apply(f);
  double approxm=m.apply(f);
  double exactVal=exact(a,b);

  cout << "MidPoint=" << approxm
       << " Simpson=" << approxs
       << " Exact=" << exactVal
       <<endl;
}
  
