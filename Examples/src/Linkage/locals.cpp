#include "globals.hpp"
#include "locals.hpp"
//! This function has local linkage
/*!
  It is not seen outside this compilation unit!
 */
namespace
{
  double fun2(double x)
  {
    // I can use a global variable!
    return Globals::globalValue*x;
  }
}

//! Also this function has local linkage!
/*!
  In C++ it is preferable to use unnamed namespaces.
 */
static double fun3(double x)
{
  return x/2;
}

//! External linkage (default)
double fun (double y)
{
  // I can use fun2 and fun3 here since thet are in the 
  // same translation unit
  // I also use the namespace global struct
  double value=fun2(y)*fun3(y)*Globals::globalParameters.g2;
  if (Globals::globalParameters.negate)
    value=-value;
  return value;
}

void zeroGlobal(){ Globals::globalValue=0;}

