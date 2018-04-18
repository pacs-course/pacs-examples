#ifndef HH_BB_HH
#define HH_BB_HH
// The class defined in this file needs the full definition of
// A since it stores an object of type A so I need to include
// the corresponding header
#include "A.hpp"
struct B
{
  int j{0};
  double x{3.15};
  double funny(){return M_a.f(*this);} 
private:
  A M_a;
};
#endif
