#ifndef HH_GLOBALS_HH
#define HH_GLOBALS_HH
//! \file globals.hpp Defines use of globals with external linkage
//! A global varibale with external linkage. Only declarated!
/*!
  Must be defined elsewhere.
 */
#ifndef NOGLOBAL
extern int globalValue;
#endif
//! A global stuct that may represent global parameters of a program
/*!
  Declaration 
 */
struct GlobalParameters
{
  bool negate;
  double g2;
};

//! A variable with external linkage only declared!
/*!
  Must be defined elsewhere.
 */
extern GlobalParameters globalParameters;
#endif
