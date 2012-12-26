#ifndef HH_MOVE_SWAP_HH__
#define HH_MOVE_SWAP_HH__
#include <type_traits>
#include <cassert>
template <class T>
class Move_swap{
public:
  static_assert(std::is_move_constructible<T>::value,
		"Move_swap requires move constructor");
  static_assert(std::is_move_assignable<T>::value,
		"Move_swap requires move assignment");
  void operator()(T& a, T& b){
    auto c(std::move(b));
    b = std::move(a);
    a = std::move(c);
  }
};
#endif
