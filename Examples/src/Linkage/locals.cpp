#include "globals.hpp"
#include "locals.hpp"
#ifdef NOGLOBAL
//! It has local linkage
static int globalValue;
#endif
//! This funcaion has local linkage
/*!
  It is not seen outside this compilation unit!
 */
namespace
{
  double fun2(double x)
  {
    //! I use a global variable!
    return globalValue*x;
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
  // I can use fu2 and fun3 here since it is the 
  // same translation unit
  double value=fun2(y)*fun3(y)*globalParameters.g2;
  if (globalParameters.negate)
    value=-value;
  return value;
}
