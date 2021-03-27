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
  std::cout << " Now with the recursive template" << std::endl;
  clock.start();
  fibo = fib<3>::value;
  clock.stop();
  std::cout << " f3 = " << fibo << " Time=" << clock.wallTime() << '\n';
  clock.start();
  fibo = fib<9>::value;
  clock.stop();
  std::cout << " f9 = " << fibo << " Time=" << clock.wallTime() << '\n';
  clock.start();
  fibo = fib<40>::value;
  clock.stop();
  std::cout << " f40 = " << fibo << " Time=" << clock.wallTime() << '\n';

  std::cout << " Now with the constexpr function and constexpr argument\n";
  clock.start();
  fibo = Fibonacci(3);
  clock.stop();
  std::cout << " f3 = " << fibo << " Time=" << clock.wallTime() << '\n';
  clock.start();
  fibo = Fibonacci(9);
  clock.stop();
  std::cout << " f9 = " << fibo << " Time=" << clock.wallTime() << '\n';
  clock.start();
  fibo = Fibonacci(40);
  clock.stop();
  std::cout << " f40 = " << fibo << " Time=" << clock.wallTime() << '\n';
  unsigned int l = 999u;
  while (true)
    {
      unsigned long int ff;
      std::cout << "Give me the index of the Fibonacci number you want (0=exit)"
                << std::endl;
      std::cin >> l;
      if (l == 0ul)
        break;
      clock.start();
      ff = Fibonacci(l);
      clock.stop();
      std::cout << " f" << l << "= " << ff << '\n';
      std::cout << "Time to evaluate F(" << l
                << ") with non constexpr argument and recursion:" << clock.wallTime()
                << std::endl;
      clock.start();
      ff = FibonacciNR(l);
      clock.stop();
      std::cout << " f" << l << "= " << ff << '\n';
      std::cout << "Time to evaluate F(" << l
                << ") with non recursive function:" << clock.wallTime() << std::endl;
    }
  return 0;
}
