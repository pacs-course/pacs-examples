
#include "globals.hpp"
#include "locals.hpp"
#include <iostream>

//! a program to show linkage
int
main()
{
  // To save typing Globals::
  using Globals::globalParameters;
  using Globals::globalValue;
  std::cout << "Global value: " << globalValue << std::endl;
  globalParameters.negate = false;
  globalParameters.g2 = -3;
  // I can use fun, but NOT fun2 nor fun3 because they have local linkage
  std::cout << "Vaule of fun(5.0) " << fun(5.0) << std::endl;
  //! I can change the value of the global variables
  globalParameters.negate = true;
  std::cout << "After changing the value of a global variable " << fun(5.0)
            << std::endl;
  // std::cout<<fun2(5.0)<<std::endl; //error
  zeroGlobal();
  std::cout << "Global value: " << globalValue << std::endl;
}
