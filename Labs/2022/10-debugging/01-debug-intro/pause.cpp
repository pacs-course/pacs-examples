#include <iostream>

int
main(int argc, char **argv)
{
  std::cout << "Before pause." << std::endl;

  // Wait for the user to hit enter.
  std::cin.get();
  std::cout << "After pause." << std::endl;

  // Pause in a loop.
  for (int i = 0; i < 5; ++i)
    {
      std::cout << "iter: " << i << std::endl;
      std::cin.get();
    }

  return 0;
}
