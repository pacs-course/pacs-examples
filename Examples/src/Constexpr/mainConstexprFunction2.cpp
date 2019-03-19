#include "twoFunctions.hpp"
#include <iostream>
int main()
{
  double a;
  std::cin >>a;
  // Here the compiler cannot know the value passed as argument to the
  // function. The function  will be
  // inlined (since constexpr implies inline) but not completely
  // resolved at compile time
  double x=cubeconst(a);
}

