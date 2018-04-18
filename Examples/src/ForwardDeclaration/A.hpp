#ifndef HH_AA_HH
#define HH_AA_HH
// I need a forward declaration
// I cannot include B.hh since class B contains an member
// of type A
class B;
//! A class with a method that takes a reference to B
class A{
public:
  //! Here the compiler needs only an incomplete (forward) declaration 
  double f(B &);
};
#endif
