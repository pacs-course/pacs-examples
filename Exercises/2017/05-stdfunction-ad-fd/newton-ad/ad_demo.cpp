#include <iostream>
#include <functional>
#include <cmath>
#include "ad.h"

int
main (void)
{
  var X, Y;

  
  Y = 3.0 * ( 4.2 * sin (X * cos (X)) * sin (X));
  auto dYdX = [] (double x) -> double
    { return 12.6*cos(x*cos(x))*sin(x)*(cos(x)-x*sin(x))+12.6*cos(x)*sin(x*cos(x)); };
  

  // Y = (X^3)*3.0 + X*5.0 + 3.0;
  // auto dYdX = [] (double x) -> double
  //   { return   (std::pow(x,2)*9.0 + 5.0); };
  
  for (double x = 0; x < 6.29; x += .01)
    std::cout << x << ", " << Y.eval (x)
              << ", " << Y.eval_der (x)
              << ", " << dYdX (x)
              << std::endl;
  
  return 0;
};
