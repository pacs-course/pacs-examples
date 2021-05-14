#include <omp.h>

#include <iostream>

int
main(int argc, char **argv)
{
  std::cout << "Hello World!" << std::endl;

#pragma omp parallel
  {
    std::cout << "Hello World (this time in parallel!)" << std::endl;
  }

  return 0;
}
