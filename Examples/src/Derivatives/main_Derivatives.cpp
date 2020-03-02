#include "Derivatives.hpp"
#include <iostream>
#include <cmath>
int main()
{
  std::function<double (const double &)> f=
    [](const double & x)
    {return std::pow(x,4);};
  double h=0.1;

  auto df= apsc::makeForwardDerivative<4>(f,h);
  std::cout<<" Approx. fourth derivative of f at x=5 is "<<df(5.)<<std::endl;
  // but I can give any function object
  auto g=[](const double & x){return std::exp(x);};
  std::cout<<" Approx. third derivative of exp(x) at x=2 is "<<
    apsc::NthDerivative<3,decltype(g)>{g,h}(2.)<<std::endl;

  
}
