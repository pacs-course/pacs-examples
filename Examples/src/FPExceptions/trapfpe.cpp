#define _GNU_SOURCE 1
#include "trapfpe.hpp"
//! Test floating point exceptions
/*
  This function throw an exception if some FPE have been raised
 */
void test_fpe_exception(bool on)
{
  using namespace std;
  if (!on) return;
  // maybe some exceptions have been raised
  // I test only the "fatal exceptions"
  auto set_excepts = fetestexcept(FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
  // clear exception flags
  feclearexcept(FE_INVALID |FE_DIVBYZERO|FE_OVERFLOW);
  switch(set_excepts){
  case FE_INVALID:
    throw InvalidFPOperation();
    break;
  case FE_OVERFLOW:
    throw FloatOverflow();
    break;
  case FE_DIVBYZERO:
    throw ZeroDivision();
    break;
  }
}


