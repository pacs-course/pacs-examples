#include <cmath>
#include <iostream>
int
main()
{
  double a = std::pow(3.0, 1. / 5);
  double b = a * a * a * a * a;
  double c = 3.0;
  if(c == b)
    std::cout << c << " and " << b << " are equal" << std::endl;
  else
    {
      std::cout << c << " and " << b << " differ" << std::endl;
      std::cout.precision(16);
      std::cout << c << " and " << b << " differ" << std::endl;
    }
}
