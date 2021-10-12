#include "twoFunctions.hpp"
int
main()
{
  const double a = 5.0;
  // Here the compiler cannot resolve statically since
  // a is not a constant expression, but a variable.
  // The function  will be
  // inlined (since constexpr implies inline) but not completely
  // resolved at compile time
  auto x = cubeconst(a);
}
