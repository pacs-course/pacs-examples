// template metaprogram for Fibonacci numbers
#include "fibonacci.hpp"
#include <iostream>


int main() {
  const int f3 = fib<3>::value;
  const int f9 = fib<9>::value;
  const int f10 =fib<10>::value;

  std::cout << " f3 = " << f3 << '\n';
  std::cout << " f9 = " << f9 << '\n';
  std::cout << " f10= " << f10 << '\n';
  std::cout<<" Now with the constexpr function"<<std::endl;
  std::cout << " f3 = " << Fibonacci(3) << '\n';
  std::cout << " f9 = " << Fibonacci(9) << '\n';
  std::cout << " f20= " << Fibonacci(20) << '\n';
  unsigned long l=999ul;
  while (true)
    {
      std::cout << "Give me the index of the Fibonacci number you want (0=exit)"<<std::endl;
      std::cin>>l;
      if(l==0ul)break;
      std::cout << " f"<<l<<"= " << Fibonacci(l) << '\n';
    }
  
  return 0;
}
