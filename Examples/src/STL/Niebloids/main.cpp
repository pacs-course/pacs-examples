#include "norm2.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ranges>
#include <vector>

int
main()
{
  using namespace apsc;

  std::vector values{3.0, 4.0};

  std::cout << "norm2({3,4}) = " << niebloids::norm2(values) << '\n';
  if(std::abs(niebloids::norm2(values) - 5.0) > 1.0e-12)
    {
      std::cerr << "Unexpected result for the vector norm\n";
      return 1;
    }

  return 0;
}
