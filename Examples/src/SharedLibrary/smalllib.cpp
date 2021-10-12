#include "smalllib.hpp"
#include <iostream>
foo::foo() : a(4), b(7)
{
  std::cout << " Building a foo object using release 0 of smalllib V. 1"
            << std::endl;
}
