#ifndef HH_FPEUTILITIES_HH
#define HH_FPEUTILITIES_HH
#include <stdexcept>
#include <string>

namespace apsc
{
//! This function may throw exceptions if a fpe has been raised.
//!
//! @param on if true means that exception will be thrown if a floating point error occurred
//! @return if on is false, it returns true if a floating point exception has occurred
bool test_fpe_exception(bool on = true);

/*!
 * @class InvalidFPOperation
 * @brief Exception for invalid floating point operations
 *
 * It launches a runtime exception with a message. It follows the
 * architecture of a standard exception (via inheritance).
 *
 */
class InvalidFPOperation : public std::exception
{
public:
  /*!
   * @brief The constructore takes t flag
   *
   * @param flags A frag that may be a combination of FE_INVALID FE_OVERFLOW or FE_DIVBYZERO
   */
  InvalidFPOperation(int flags);
  /*!
   * @return The message to be printed
   */
  const char *what() const noexcept override;

private:
  std::string message;
};
} // namespace apsc
#endif
