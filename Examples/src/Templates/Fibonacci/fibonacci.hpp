#ifndef  H_FIBONACCI_H
#define H_FIBONACCI_H
// ! Fibonacci class. Compatible with C++98
template<unsigned int N> struct fib{
  static constexpr unsigned long int value = fib<N-1>::value + fib<N-2>::value;
};

//! Specialization for 1
template<> struct fib<1>{
  static constexpr unsigned long int value = 1ul;
};

//!Specializatio for 0
template<> struct fib<0>{
  static constexpr unsigned long int value = 0ul;
};


/*!
  @brief Fibonacci template function.
  
  @detail It implements a constexpr function. The advantage is that it works
  also with non constant expression arguments. In that case it is
  evaluated run time. 
  
  While, if the argument is a constant expression the function all is
  resolved at compile time.

  @note beware of integer overflow!! Fibonacci numbers grow fast!
*/
constexpr unsigned long int Fibonacci(unsigned int const N)
{
  if (N <=2ul)
    return 1ul;
  else
    return Fibonacci(N-1)+Fibonacci(N-2);
}
//! The recursive version is heavy for "large" N So here I have a non recursive version
//! Definition in the cpp file
unsigned long int FibonacciNR(unsigned int const N);

#endif
