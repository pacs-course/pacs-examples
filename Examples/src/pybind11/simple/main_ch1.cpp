#include <iostream>  // include a standard library for input/output
#include "sumInt.hpp"
int
main()
{
  using std::cin; // brings std:;cin in the current scope
  int n, m;            // declare n and m to be integer
  // I have not brought cout and endl in the current scope: i use the
  // full qualified name
  std::cout << "Enter two integers:" << std::endl; // output to screen
  cin >> n >> m;                         // input from terminal
  auto sum =sumInt(m,n);
  std::cout << "The sum of integers from " << n << " to " << m << " is: " << sum
       << std::endl; // output sum to screen
}
// Note: Using endl prints out a newline and flushes the output buffer.
// Alternatively you may use "\n".
// We have used the brace initialization for sum
// Alternatively you may use:
// int sum(0); // old style initialization (but still perfectly valid)
// int sum = 0;// Equivalent to int sum(0);
