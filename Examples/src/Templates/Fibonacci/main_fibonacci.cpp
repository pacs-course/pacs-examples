// template metaprogram for Fibonacci numbers
#include "chrono.hpp"
#include "fibonacci.hpp"
#include <iostream>

int
main()
{
  Timings::Chrono clock;
  std::cout << " Time is in microseconds\n";
  unsigned long int fibo;
  constexpr unsigned long int N=10ul;
  {  
  std::cout << " With the recursive template class" << std::endl;
  clock.start();
  //fibo = fib<N>::value; // old way
  fibo=fib_v<N>; // using the template variable
  clock.stop();
  std::cout << " F"<<N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';
  clock.start();
  fibo = fib<2*N>::value;
  clock.stop();
  std::cout << " F"<<2*N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';
  clock.start();
  fibo = fib<4*N>::value;
  clock.stop();
  std::cout << " F"<<4*N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';
  }
  {  
  std::cout << " With the recursive template function" << std::endl;
  clock.start();
  fibo=FibonacciR<N>();
  clock.stop();
  std::cout << " F"<<N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';
  clock.start();
  fibo=FibonacciR<2*N>(); 
  clock.stop();
  std::cout << " F"<<2*N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';
  clock.start();
  fibo=FibonacciR<4*N>();
  clock.stop();
  std::cout << " F"<<4*N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';
  }
  {
  std::cout << " Now with the recursive constexpr function and constexpr argument\n";
  clock.start();
  fibo = Fibonacci(N);
  clock.stop();
  std::cout << " F"<<N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';
  clock.start();
  fibo = Fibonacci(2*N);
  clock.stop();
  std::cout << " F"<<2*N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';
  clock.start();
  fibo = Fibonacci(4*N);
  clock.stop();
  std::cout << " F"<<4*N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';
  }
  {
  std::cout << " Now with the non recursive function and constexpr argument\n";
    clock.start();
    fibo = FibonacciNR(N);
    clock.stop();
    std::cout << " F"<<N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';

    clock.start();
    fibo = FibonacciNR(2*N);
    clock.stop();
    std::cout << " F"<<2*N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';

    clock.start();
    fibo = FibonacciNR(4*N);
    clock.stop();
    std::cout << " F"<<4*N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';

  }
{
  std::cout << " Now with the recursive function and non constexpr\n";
    auto M=N;
    clock.start();
    fibo = Fibonacci(M);
    clock.stop();
    std::cout << " F"<<M<<" = " << fibo << " Time=" << clock.wallTime() << '\n';

    clock.start();
    M*=2;
    fibo = Fibonacci(M);
    clock.stop();
    std::cout << " F"<<M<<" = " << fibo << " Time=" << clock.wallTime() << '\n';

    clock.start();
    M*=2;
    fibo = Fibonacci(4*N);
    clock.stop();
    std::cout << " F"<<4*N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';

  }
{
  std::cout << " Now with the non recursive function and non constexpr\n";
    auto M=N;
    clock.start();
    fibo = FibonacciNR(M);
    clock.stop();
    std::cout << " F"<<M<<" = " << fibo << " Time=" << clock.wallTime() << '\n';

    clock.start();
    M*=2;
    fibo = FibonacciNR(M);
    clock.stop();
    std::cout << " F"<<M<<" = " << fibo << " Time=" << clock.wallTime() << '\n';

    clock.start();
    M*=2;
    fibo = FibonacciNR(4*N);
    clock.stop();
    std::cout << " F"<<4*N<<" = " << fibo << " Time=" << clock.wallTime() << '\n';

  }
  return 0;
}
