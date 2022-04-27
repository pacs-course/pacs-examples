#include "debug_macro.hpp"

#include <iostream>

int
main()
{
  using std::cout;
  using std::endl;

  int a = 5;
  int b = 6;

  BUGPRINT(a * b);

  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;

  std::cout << "2 * (" << a << " + " << b << ") = " << TWO_X(a + b)
            << std::endl;

  return 0;
}
