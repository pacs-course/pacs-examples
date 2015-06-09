#include "Pow.hpp"
#include <iostream>
int main()
{
  std::cout<< Pow<11l,11>()<<std::endl;
  std::cout<< pow<11>(11l)<<std::endl;
}
