
#include <iostream>
#include "globals.hpp"
#include "locals.hpp"
#include "locals2.hpp"
// define globals
GlobalParameters globalParameters;
int globalValue=5;// I can also initialize the global
//! a program to show linkage
int main()
{
  std::cout<<"Global value: "<<globalValue<<std::endl;
  globalParameters.negate=false;
  globalParameters.g2=-3;
  // I can use fun, but NOT fun2 or fun3 becouse they have local linkage
  std::cout<<fun(5.0)<<std::endl;
  //! I can change the value of the global variables
  globalParameters.negate=true;
  std::cout<<fun2(5.0)<<std::endl;
  //std::cout<<fun2(5.0)<<std::endl; //error local linkage only
  zeroGlobal();
  std::cout<<"Global value: "<<globalValue<<std::endl;
}
