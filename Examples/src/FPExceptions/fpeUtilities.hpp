#ifndef HH_FPEUTILITIES_HH
#define HH_FPEUTILITIES_HH
#include <stdexcept>

//!This function may throw exceptions if a fpe has been raised.
//! @param on if true means that exceptions will be thrown
//! @return if on is false, returns true if a floating point exception has occurred
bool test_fpe_exception(bool on=true);

//! Exception for invalid operations
/*
  It launches a runtime exception with a message.
 */
struct 
InvalidFPOperation: public std::runtime_error
{
  InvalidFPOperation():std::runtime_error("Invalid floating point operation"){};
};

//! Exception for floating point overflow
/*
  It launches a runtime exception with a message.
 */
struct 
FloatOverflow: public std::runtime_error
{
  FloatOverflow():std::runtime_error("Floating point overflow"){};
};

//! Exception for zero division
/*
  It launches a runtime exception with a message.
 */
struct 
ZeroDivision: public std::runtime_error
{
  ZeroDivision():std::runtime_error("Division by zero"){};
};


#endif
