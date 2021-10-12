#ifndef HH_AA_HH
#define HH_AA_HH
// I need a forward declaration
// I cannot include B.hh since class B contains an member
// of type A

class B; // Try to take it out
// This is not good, try it
// #include "B.hpp"
//! A class with a method that takes a reference to B
class A
{
public:
  //! Here the compiler needs only an incomplete (forward) declaration
  //! Beware: Cannot have definition here! In the definition I use
  //! an object of type B and in this case the compiler needs a full
  //! declaration!
  double f(B &);
};
#endif
