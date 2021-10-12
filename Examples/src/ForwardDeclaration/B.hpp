#ifndef HH_BB_HH
#define HH_BB_HH
// The class defined in this file needs the full definition of
// A since it stores an object of type A so I need to include
// the corresponding header
#include "A.hpp"
// class A; // try just a forward declaration (comment #include A.hpp)
struct B
{
  int    j{0};
  double x{3.15};
  //! Pure declaration. This function uses M_a, of type A, but here I
  //! could have put an in-class definition since the complete
  //! definition of A is visible to the compiler.
  double funny();

private:
  A M_a;
};
#endif
