#ifndef HH_ENABLE_SHARED_HH
#define HH_ENABLE_SHARED_HH
#include <memory>
//! an example of use of shared_from_this
//! Note the CRTP technique!
class Try : public std::enable_shared_from_this<Try>
{
public:
  double
  get() const
  {
    return a;
  }
  void
  set(const double &b)
  {
    a = b;
  }

private:
  double a = {0};
};
#endif
