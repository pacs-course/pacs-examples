/*---------------------------------------------------------------------*
| header file  extendedAssert                                          |
| #Purposes Defines typedefs and macros for debugging                  |
| Copyright Luca Formaggia 2005                                        |
-----------------------------------------------------------------------*/
/*! @file extendedAssert.hpp
    @brief some useful macros that extend the classic assert()

    @detail This file introduces some C preprocessor macros that work
    similarly to the standard assert() macro but they allow a finer control.
    The macros may be activated/deactivated by defining suitable preprocessor
variables

    The macros defined in this file are:

  - INLINE    if enabled converts to null, otherwise converts to inline;
  - SURE_ASSERT(cond,"mesg")     prints mesg if cond==false, prints also
                                 line number and file name. Cannot be disabled;
  - ASSERTM(cond,"mesg")         as SURE_ASSERT but may be disabled;
  - ASSERT(cond)                 as ASSERTM with no message;
  - PRE_ASSERT(cond,"mesg")      as ASSERTM;
  - POST_ASSERT(cond,"mesg")     as ASSERTM;
  - INV_ASSERT(cond,"mesg")      as ASSERTM.

 The intent is to use the XXX_ASSERT version to test pre, post conditions and
invariants, respectivly, and be able to swich on and off the testing selectively
by using preprocessor variable bassed via the -D option

 Default values

 By default all macros are enabled

 Preprocessor variables that affect the asserts

  - NDEBUG      All macros are disabled apart SURE_ASSERT;
  - INLINED     INLINE is disabled (i.e. it converts to inline!);
  - NOASSERT    ASSERTM disabled;
  - NOXXX where XXX is either PRECON or POST or INV: corresponding ASSERT
              is disactivated;
  - ERRORSTATUS Could be defined as an integer number passed to exit (defaulted
to 1).

Example of usage:

@code
SURE_ASSERT(m>0,"m should be positive")
@endcode
*/
#ifndef _EXTENDEDASSERT_HH_
#define _EXTENDEDASSERT_HH_

// This test assumes you are compiling with gnu C++ compiler
// or any other compiler which sets __cplusplus when the C++ compiler
// is launched

#ifndef __cplusplus
#error You must use C++
#endif
#include <cstdlib>
#include <iostream>

#ifdef NDEBUG
#define INLINED
#define NOASSERT
#define NOPRE
#define NOPOST
#define NOINV
#endif

#ifdef INLINED
#define INLINE inline
#else
#define INLINE
#endif

#ifndef ERRORSTATUS
#define ERRORSTATUS 1
#endif

//
#define SURE_ASSERT(X, A)                                                      \
  if(!(X))                                                                     \
    do                                                                         \
      {                                                                        \
        std::cerr << std::endl                                                 \
                  << A << std::endl                                            \
                  << " In file " << __FILE__ << " line " << __LINE__ << " : "; \
        std::exit(ERRORSTATUS);                                                \
      }                                                                        \
    while(0);

#ifndef NDEBUG
#define ASSERT(X) ASSERTM(X, " ")
#else
#define ASSERT(X)
#endif

#ifdef NOASSERT
#define ASSERTM(X, A)
#else
#define ASSERTM(X, A) SURE_ASSERT(X, A)
#endif

#ifdef NOPRE
#define PRE_ASSERT(X, A)
#else
#define PRE_ASSERT(X, A) SURE_ASSERT(X, A)
#endif

#ifdef NOPOST
#define POST_ASSERT(X, A)
#else
#define POST_ASSERT(X, A) SURE_ASSERT(X, A)
#endif

#ifdef NOINV
#define INV_ASSERT(X, A)
#else
#define INV_ASSERT(X, A) SURE_ASSERT(X, A)
#endif

//  end of  _EXTENDEDASSERT_HH_
#endif
