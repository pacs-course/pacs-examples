#ifndef HH_GLOBALS_HH
#define HH_GLOBALS_HH
//! \file globals.hpp Defines use of variables with external linkage

namespace Globals
{
//! A global (namespace) variable with external linkage. Only declarated!
/*!
  Must be defined elsewhere. It would have been better to create a namespace
  (see below)
 */
extern int globalValue;
//! Declaration of a global stuct that may represent global parameters of a program
/*!
    It is a good practice to use namespaces. In this case the global variable
    is a namespace variable.
 */
  struct GlobalParameters
  {
    bool negate;
    double g2;
  };
  
  //! A namespace variable with external linkage: only declared!
  /*!  Must be defined elsewhere, typically in the correspining source
    file.  This variable will be available to all translation units
    including this file (and linking with the source). Maybe you want to make
    GlobalParameters a Singleton, we will discuss that in a specific lecture.
  */
  extern GlobalParameters globalParameters;
}// end namespace Globals
#endif
