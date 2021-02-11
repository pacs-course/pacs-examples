#include "Pi.hpp"
#include <iostream>
#include <limits>

int
main()
{
  using namespace apsc::MathConstants;
  std::cout.precision(std::numeric_limits<float>::digits10 + 1);
  std::cout << "pi (float): " << Pi_f << std::endl;
  std::cout.precision(std::numeric_limits<double>::digits10 + 1);
  std::cout << "pi (double): " << Pi << std::endl;
  std::cout.precision(std::numeric_limits<long double>::digits10 + 1);
  std::cout << "pi (long double): " << Pi_l << std::endl;
}
