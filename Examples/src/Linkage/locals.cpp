#include "globals.hpp"
//! This function has local linkage since is defined in a anonymous namespace
/*!
  It is not seen outside this compilation unit!
  @note no declaration in the header file!
 */
namespace
{
double
fun2(double x)
{
  // I can use a global variable!
  return Globals::globalValue * x;
}
} // namespace

//! Also this function has local linkage thanks to the static attribute
/*!
  In C++ it is preferable to use unnamed namespaces.
  @note no declaration in the header file!
 */
static double
fun3(double x)
{
  return x*Globals::Pi / 2;
}

//! External linkage (default)
double
fun(double y)
{
  // I can use fun2 and fun3 here since they are in the
  // same translation unit
  double value = fun2(y) * fun3(y) * Globals::globalParameters.g2;
  // Again a can use a Global variable
  if(Globals::globalParameters.negate)
    value = -value;
  return value;
}

//! a function that operated on global variables
void
zeroGlobal()
{
  Globals::globalValue = 0;
  Globals::globalParameters.negate = true;
}
