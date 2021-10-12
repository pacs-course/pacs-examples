#include "B.hpp"
//! this definition could have ben done in class, since it is very
//! short and can be easily inlined, but I put it in a cpp file just
//! to show that the problem of forward declaration not being possible
//! is not related to the definition of the method.
double
B::funny()
{
  return M_a.f(*this);
}
