#include "twoFunctions.hpp"
#include <iostream>
int main()
{
  double a;
  std::cin >>a;
  // Here the compilar cannot guess the value of a
  // function will be inlined but not resolved compile time
  double x=cubeconst(a);
}

