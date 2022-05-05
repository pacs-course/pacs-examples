#include <cassert>
#include <iostream>

/**
 * Add two non-negative integers.
 * @param x integer to add.
 * @param y integer to add.
 * @return the value of x+y.
 *
 * @pre x >= and y >= 0.
 */
int
non_negative_add(const int &x, const int &y)
{
  assert(x >= 0);
  assert(y >= 0);

  return x + y;
}

int
main()
{
  std::cout << non_negative_add(2, 2) << std::endl;
  std::cout << non_negative_add(2, -5) << std::endl;
}
