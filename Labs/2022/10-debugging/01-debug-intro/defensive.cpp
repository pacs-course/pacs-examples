#include <iostream>

// This function should assign (a + b) to b.
// a should be marked as "const"!
void
add(int &a, int &b)
{
  a += b;
}

int
main(int argc, char **argv)
{
  // Another "const" missing here!
  int x = 2;
  int y = 3;

  std::cout << x << " + " << y << " = ";
  add(x, y);
  std::cout << y << std::endl;

  return 0;
}
