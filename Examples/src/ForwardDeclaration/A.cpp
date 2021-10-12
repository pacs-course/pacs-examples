#include "A.hpp"
// Here I need the full definition of B
// since in the definition of the function I use
// an object of type B, so the compiler must have
// the full declaration visible.
#include "B.hpp"
double
A::f(B &b)
{
  b.j += 2;
  return b.x;
}
