#include <iostream>

int
main()
{
  std::cout << "Hello World!" << std::endl;

#pragma omp parallel num_threads(8)
  {
    std::cout << "Hello World (this time in parallel!)" << std::endl;
  }

  return 0;
}
