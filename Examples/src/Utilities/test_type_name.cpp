#include "type_name.hpp"
#include <iostream>
int
main()
{
  std::cout << apsc::type_name<int>() << std::endl;
  std::cout << apsc::type_name<double>() << std::endl;
  double *x = new double[10];
  std::cout << apsc::type_name<decltype(x)>() << std::endl;
  std::cout << apsc::type_name<std::string>() << std::endl;
  return 0;
}