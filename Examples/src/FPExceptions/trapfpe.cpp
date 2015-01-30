#define _GNU_SOURCE 1
#include "trapfpe.hpp"
//! Test floating point exceptions
/*
  This function throw an exception if some FPE have been raised
 */
bool test_fpe_exception(bool on)
{
  using namespace std;
    // maybe some exceptions have been raised
  // I test only the "fatal exceptions"
  auto set_excepts = fetestexcept(FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
  // clear exception flags
  feclearexcept(FE_INVALID |FE_DIVBYZERO|FE_OVERFLOW);
  switch(set_excepts){
  case FE_INVALID:
    if (!on) return true;
    throw InvalidFPOperation();
    break;
  case FE_OVERFLOW:
    if (!on) return true;
    throw FloatOverflow();
    break;
  case FE_DIVBYZERO:
    if (!on) return true;
    throw ZeroDivision();
    break;
  }
  return false;
}


