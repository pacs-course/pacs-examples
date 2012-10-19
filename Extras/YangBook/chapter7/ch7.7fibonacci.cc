// template metaprogram for Fibonacci numbers

#include <iostream>

template<int N> struct fib{
  enum { value = fib<N-1>::value + fib<N-2>::value};
};

template<> struct fib<1>{
  enum { value = 1 };
};

template<> struct fib<0>{
  enum { value = 0 };
};

// we may also define static constants for value
template<int N> struct fib2 {
  static const int value = fib2<N-1>::value + fib2<N-2>::value;
};

template<> struct fib2<1> {
  static const int value = 1;
};

template<> struct fib2<0> {
  static const int value = 0;
};


int main() {
  const int f3 = fib<3>::value;
  const int f9 = fib<9>::value;
  const int b9 = fib2<9>::value;

  std::cout << " f3 = " << f3 << '\n';
  std::cout << " f9 = " << f9 << '\n';
  std::cout << " b9 = " << b9 << '\n';
}
