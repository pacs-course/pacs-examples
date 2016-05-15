b#ifndef  H_FIBONACCI_H
#define H_FIBONACCI_H
// ! Fibonacci class. Compatible with C++98
template<int N> struct fib{
  enum { value = fib<N-1>::value + fib<N-2>::value};
};

//! Specialization for 1
template<> struct fib<1>{
  enum { value = 1 };
};

//!Specializatio for 0
template<> struct fib<0>{
  enum { value = 0 };
};


/*!
  Fibonacci template function (only C++11).
  
  It implements function template specialization and uses
  the new constexpr keyword to indicate that we are in fact returning
  a constant expression.
*/

template<unsigned int N>
constexpr unsigned int Fibonacci(){
  return Fibonacci<N-1>()+Fibonacci<N-2>();
}

template<>
constexpr unsigned int Fibonacci<1>(){
  return 1;
}

template<>
constexpr unsigned int Fibonacci<0>(){
  return 0;
}


#endif
