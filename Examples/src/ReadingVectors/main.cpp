#include "ReadingVector.hpp"
#include <iostream>
#include <vector>

int
main()
{
  using namespace apsc::Utilities;
  { // block for laziness
    std::vector<int> a;
    a << 1, 2, 3, 4; // eigen style
    for(auto v : a)
      std::cout << v << ", ";
    std::cout << std::endl;
  }
  {
    // but you can do also simply
    std::vector<int> a;
    a << 1 << 2 << 3 << 4; // Using  vector as a stream
    for(auto v : a)
      std::cout << v << ", ";
    std::cout << std::endl;
  }
}
