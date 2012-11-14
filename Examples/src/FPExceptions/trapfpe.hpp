#ifndef __TRAPFPE_HPP__
#define __TRAPFPE_HPP__
#include <cfenv>
/*! \file trapfpe.hpp An example of use of floating point environment
  
  This file does two things. If included in your program it makes sure that
  floating point access is activated by calling a compiler pragma (C++11).
  In addition, if the cpp macro FPE_ABORT is set, it will make sure
  that a floating point exceptions lauches an exception object of type.
 */
#include "myexceptions.hpp"
// The C++11 standard requires this pragma to be activated
// Only some compilers have this feature however.
#pragma STDC FENV_ACCESS ON
#ifdef FPE_ABORT
#warning "ABORTING ON FPE IS ACTIVATED" 
//! If FPE_ABORT is set a fp exception causes the progam to terminate
/* C style implementation
   static void __attribute__ ((constructor))
   trapfpe (){
   //! Enable some exceptions.  At startup all exceptions are masked.
feenableexcept (FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
}
*/
//! IMPLEMENTATION AS STATIC METHOD (More C++ style)

namespace{
  struct FpeTrap{
    static void __attribute__ ((constructor))
    trapfpe (){
      /*! Enable some exceptions.  At startup all exceptions are masked.  */
      feenableexcept (FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
    }
  };
}
#endif

//!This function throw exceptions if a fpe has been raised
// \param on if true means that exceptions will be thrown
void test_fpe_exception(bool on=true);
#endif
