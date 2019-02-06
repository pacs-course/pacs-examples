
#include <iostream>
#include "globals.hpp"
#include "locals.hpp"
//! a program to show linkage
int main()
{
  // To save typing
  using Globals::globalParameters;
  std::cout<<"Global value: "<<globalValue<<std::endl;
  globalParameters.negate=false;
  globalParameters.g2=-3;
  // I can use fun, but NOT fun2 or fun3 becouse they have local linkage
  std::cout<<"Vaule of fun(5.0) "<<fun(5.0)<<std::endl;
  //! I can change the value of the global variables
  globalParameters.negate=true;
  std::cout<<"After changing the value of a global variable "<<fun(5.0)<<std::endl;
  //std::cout<<fun2(5.0)<<std::endl; //error local linkage only
  zeroGlobal();
  std::cout<<"Global value: "<<globalValue<<std::endl;
}
