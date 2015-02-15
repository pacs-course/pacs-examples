#include "globals.hpp"
#include "locals.hpp"
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
static double fun3(double x)
{
  return x/2;
}

//! External linkage (default)
double fun (double y)
{
  double value=fun2(y)*fun3(y)*globalParameters.g2;
  if (globalParameters.negate)
    value=-value;
  return value;
}
