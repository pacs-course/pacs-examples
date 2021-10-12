#include <iostream>
#include <limits>
// Integer overflow produce unwanted results. And they cannot be detected!
// Normally is not a problem... but beware!

int
main()
{
  int imax = std::numeric_limits<int>::max();
  int imin = std::numeric_limits<int>::min();
  std::cout << "Max int + 1= " << imax + 1 << std::endl;
  std::cout << "Min int - 1= " << imin - 1 << std::endl;
  // also underflow of unsigned int give unexpected
  // results, and it is undetected!
  unsigned int uintmin = 0;
  // Now I subtract 1
  uintmin -= 1u;
  std::cout << "Unsigned underflow= 0u-1u " << uintmin << std::endl;
}
