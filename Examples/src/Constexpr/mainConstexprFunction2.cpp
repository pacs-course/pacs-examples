#include "twoFunctions.hpp"
#include <iostream>
int main()
{
  double a;
  std::cin >>a;
  // Here the compiler cannot guess the value of a function will be
  // inlined (since it is very simple) but not resolved at compile
  // time
  double x=cubeconst(a);
}

