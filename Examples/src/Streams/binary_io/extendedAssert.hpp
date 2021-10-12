/*---------------------------------------------------------------------*
| header file  extendedAssert                                          |
| #Purposes Defines typedefs and macros for debugging                  |
| Copyright Luca Formaggia 2005                                        |
-----------------------------------------------------------------------*/
/*!
  This file introduces some extension of the assert macros which may be
  used to help debugging.

  The macros may be deactivated by defining suitable preprocessor variables

  The macros defined in this file are

        INLINE                         if enabled converts to null,
                                 otherwise converts to inline
        SURE_ASSERT(cond,"mesg")       prints mesg if cond==false, prints also
                                 line number and file name
        ASSERTM(cond,"mesg")           as SURE_ASSERT
        ASSERT(cond)                   as SURE_ASSERT with no message
  PRE_ASSERT(cond,"mesg")        as SURE_ASSERT
  POST_ASSERT(cond,"mesg")       as SURE_ASSERT
  INV_ASSERT(cond,"mesg")        as SURE_ASSERT

 The intent is to use the XXX_ASSERT version to test pre, post conditions and
invariants, respectivly, and be able to swich on and off the testing selectively
by using preprocessor variable bassed via the -D option

 Default values

 By default all macros are enabled

 Preprocessor Variables

  NDEBUG      All macros are disabled a part SURE_ASSERT,
  INLINED     INLINE is enabled
  NOASSERT    ASSERTM disabled
  NOXXX where XXX is either PRECON or POST or INV: corresponding ASSERT
              is disactivated.
  ERRORSTATUS Should be defined as an integer number passed to exit (defaulted
to 1)

It follows that  SURE_ASSERT is never disabled (as the name says)
*----------------------------------------------------------------------*/
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
