#ifndef __HH_REFWRAP_HH__
#define __HH_REFWRAP_HH__
namespace apsc
{
/*
  @file refwrap.hpp
  Some example of use of reference wrappers
*/

//! A function taking a reference.
void timesThree(double &r);

// A template function operating on a value
template <class F, class T>
void
apply(F f, T c)
{
  f(c);
}
} // end namespace apsc
#endif
