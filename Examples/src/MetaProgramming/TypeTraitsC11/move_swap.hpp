#ifndef HH_MOVE_SWAP_HH__
#define HH_MOVE_SWAP_HH__
#include <type_traits>
#include <cassert>
//! Swap function
/*
  If the class has a move constructor and a move assignment it is 
  used instead of the copy equivalent
  Note the std::swap is now defined this way!
  @param a lvalue reference of type T
  @param b lvalue refetence of type T
*/
template <class T>
void Swap (T&& a, T&& b)
{
  // If move constructor is defined it is used. Otherwise
  // c is copyconstructed
  auto c(std::move(b));
  // If move assigment is defined it is used, otherwise
  // copy assignment is used.
  b = std::move(a);
  a = std::move(c);
}
#endif
