#include <iostream>
#include "globals.hpp"
#include "locals.hpp"
// define globals
GlobalParameters globalParameters;
int globalValue=5;// I ca also initialize the global
//! a program to show linkage
int main()
{
  globalParameters.negate=false;
  globalParameters.g2=-3;
  // I can use fun, but NOT fun2 or fun3 becouse they have local linkage
  std::cout<<fun(5.0)<<std::endl;
  //! I can change the value of the global variables
  globalParameters.negate=true;
  std::cout<<fun(5.0)<<std::endl;
  //std::cout<<fun2(5.0)<<std::endl; //error local linkage only
}
