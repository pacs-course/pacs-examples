#include "fpeUtilities.hpp"
#include <cfenv>
// The C++11 standard requires this pragma to be activated
// Only some compilers have this feature however.
// Ignore possible warnings
#pragma STDC FENV_ACCESS ON
namespace apsc
{
InvalidFPOperation::InvalidFPOperation(int flags):message{"FPE exceptions:"}
{
  if (flags & FE_INVALID)
    message +=" Invalid Operation;";
  if (flags & FE_OVERFLOW)
    message +=" Overflow;";
  if (flags & FE_DIVBYZERO)
    message +=" Division by Zero;";
}

const char* InvalidFPOperation::what()const noexcept
{
  return message.c_str();
}


// Test floating point exceptions
bool test_fpe_exception(bool on)
{
  using namespace std;
    // maybe some exceptions have been raised
  // I test only the "fatal exceptions"
  auto set_excepts = fetestexcept(FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
  // clear exception flags
  feclearexcept(FE_INVALID |FE_DIVBYZERO|FE_OVERFLOW);
  if(set_excepts !=0)
    {
    if (!on) return true;
    throw InvalidFPOperation(set_excepts);
    }
  else
    return false;
}
}// end namespace

