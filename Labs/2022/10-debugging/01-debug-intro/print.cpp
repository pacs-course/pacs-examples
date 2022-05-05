#include <iostream>

int
main()
{
// Compile with `-DMY_DEBUG` to enable printing debug output
#ifdef MY_DEBUG
  std::cout << "__FILE__: " << __FILE__ << std::endl;
  std::cout << "__LINE__: " << __LINE__ << std::endl;
#endif

  return 0;
}
