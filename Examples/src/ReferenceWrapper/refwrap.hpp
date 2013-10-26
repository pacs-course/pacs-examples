#ifndef __HH_REFWRAP_HH__
#define __HH_REFWRAP_HH__
/*
@file refwrap.hpp
Some example of use of reference wrappers
*/

//! A function taking a reference.
void timesThree(double & r);

//A template function operating on a container.
template <class F, class T>
void apply(F f, T c)
{
  // c++11 only syntax
  f(c); 
}
#endif
