#ifndef  H_FIBONACCI_H
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
  @brief Fibonacci template function (since C++11).
  
  @detail It implements a constexpr function. The advantage is that it works
  also with non constant expression arguments. In that case it is
  evaluated run time. 
  
  While, if the argument is a constant expression the function all is
  resolved at compile time.

  @note beware of integer underflow if you use unsigned int. Here there is no problem
  but in general make sure that you will never compute 0u -1
*/
constexpr unsigned int Fibonacci(unsigned int N)
{
  return N <=2u ? 1u: Fibonacci(N-1)+Fibonacci(N-2);
}


#endif
