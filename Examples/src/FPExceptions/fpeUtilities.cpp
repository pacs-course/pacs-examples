#include "fpeUtilities.hpp"
#include <cfenv>
// The C++11 standard requires this pragma to be activated
// Only some compilers have this feature however.
// Ignore possible warnings
#pragma STDC FENV_ACCESS ON

// Test floating point exceptions
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


