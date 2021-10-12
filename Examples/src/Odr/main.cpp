#include "fun.hpp"
#include "fun2.hpp"
#include <iostream>
int
main()
{
  double y = fun2(1.0);
  double z = fun(1.0);
  std::cout << y << " " << z << std::endl;
}
