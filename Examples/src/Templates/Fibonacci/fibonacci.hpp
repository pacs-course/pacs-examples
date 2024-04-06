#ifndef H_FIBONACCI_H
#define H_FIBONACCI_H
/*!
Different implementation of tools to compute fibonacci
numbers
*/


/*! Fibonacci class template. Compatible with C++98
Here the indication of the desired Fibonacci number is a template
parameter. Thus it must be known t compile time
*/
template <unsigned int N> struct fib
{
  static constexpr unsigned long int value =
    fib<N - 1>::value + fib<N - 2>::value;
};

//! Specialization for 1
template <> struct fib<1>
{
  static constexpr unsigned long int value = 1ul;
};

//! Specialization for 0
template <> struct fib<0>
{
  static constexpr unsigned long int value = 0ul;
};

//! This is just to show a use of a tempalte variable
  template <unsigned long N>
  unsigned long fib_v =fib<N>::value;
/*!
 A more modern way of writing the template. A recursive function template
 that exploits if constexpr to break the recursion
*/
template <long unsigned int N>
auto FibonacciR()
{
  if constexpr (N==0ul)
    return 0ul;
  else if constexpr (N==1ul)
    return 1ul;
  else
    return FibonacciR<N - 1>() + FibonacciR<N - 2>();
}

/*!
  @brief Fibonacci constexpr function.

  @detail It implements a constexpr function. The advantage is that it works
  also with non constant expression arguments. In that case it is
  evaluated run time. Yet, it may also work if the argument is not a 
  constant expression. In which case it bahaves as a normal inline function

  While, if the argument is a constant expression the function all is
  resolved at compile time.

  @note beware of integer overflow!! Fibonacci numbers grow fast!
*/
constexpr unsigned long int
Fibonacci(unsigned int const N)
{
  if (N == 1ul)
    return 1ul;
  else if (N==0ul)
    return 0ul;
  else
    return Fibonacci(N - 1ul) + Fibonacci(N - 2ul);
}
//! The recursive version is heavy for "large" N So here I have a non recursive
//! version 
constexpr unsigned long int
FibonacciNR(unsigned int const N)
{
  unsigned long int first = 1ul;
  unsigned long int second = first;
  if(N > 3ul)
    {
      for(long unsigned int i = 3ul; i <= N; ++i)
        {
          auto save = second;
          second += first;
          first = save;
        }
    }
  return second;
}

#endif
