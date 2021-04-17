//#define _GNU_SOURCE 1
#ifndef __TRAPFPE_HPP__
#define __TRAPFPE_HPP__
/*! \file trapfpe.hpp An example of use of floating point environment
  
  If included in your main program it ensures that
  if the cpp macro FPE_ABORT  is set using -DFPE_ABORT,
  a floating point exceptions causes the program to abort.
 */
#ifdef FPE_ABORT
#include <cfenv>
// The C++11 standard requires this pragma to be activated
// Only some compilers have this feature however.
// Ignore possible warnings
#pragma STDC FENV_ACCESS on
#warning "ABORTING ON FPE ACTIVATED: "
#warning "FE_INVALID FE_DIVBYZERO FE_OVERFLOW are trapped" 
//! If FPE_ABORT is set a fp exception causes the progam to terminate
/* C style implementation */
//   static void __attribute__ ((constructor))
//   trapfpe (){
   //! Enable some exceptions.  At startup all exceptions are masked.
//     feenableexcept (FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
//}
//
//! IMPLEMENTATION IN AN ANONYMOUS NAMESPACE (More C++ style)
  namespace
{
  struct FpeTrap{
    static void __attribute__ ((constructor))
    trapfpe (){
      //! Enable some exceptions.  At startup all exceptions are masked.
      feenableexcept (FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
    }
  };
}
#endif

#endif
