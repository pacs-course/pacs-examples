#ifndef HH_FPEUTILITIES_HH
#define HH_FPEUTILITIES_HH
#include <stdexcept>
#include <string>

namespace apsc
{
//!This function may throw exceptions if a fpe has been raised.
//! @param on if true means that exceptions will be thrown
//! @return if on is false, returns true if a floating point exception has occurred
bool test_fpe_exception(bool on=true);

//! Exception for invalid operations
/*
  It launches a runtime exception with a message.
 */
class
InvalidFPOperation: public std::exception
{
public:
  InvalidFPOperation(int flags);
  const char* what()const noexcept override;
private:
  std::string message;
};
}// end namespace
#endif
