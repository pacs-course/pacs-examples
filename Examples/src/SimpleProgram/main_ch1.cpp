/* A simple program to illustrate some basic features of C++. It adds all
   integers between two given integers and outputs the sum to the screen. */

#include <algorithm> // for std::swap
#include <format>    // use noew c++20 format
#include <iostream>  // include a standard library for input/output
int
main()
{
  // using namespace std; // bring std names on the current namespace (avoid
  // doing it)
  using std::cin;  // brings std:;cin in the current scope
  using std::swap; // brings swap in the current scope
  int n, m;        // declare n and m to be integer
  // I have not brought cout and endl in the current scope: i use the
  // full qualified name
  std::cout << "Enter two integers:" << std::endl; // output to screen
  cin >> n >> m;                                   // input from terminal
  if(n > m)
    swap(n, m); // if n is bigger than m, swap them
  int sum{0};   // sum is initialized to 0.
  // other ways to initialize sum (C++ loves having different way of doing the
  // same thing) are at the bottom of the file a loop, i changes from n to m
  // with increment 1 each time
  for(auto i = n; i <= m; ++i) // automatic deduction of type for i
    sum += i; // I do not need opening a scope with {} since it is just 1 line
  // but doing it is not wrong:
  // for (auto i = n; i <= m; ++i) {
  //   sum += i;
  // }

  std::cout << std::format("The sum of integers from {} to {} is: {}", n, m,
                           sum)
            << std::endl;
  /*
  // without std::format
  std::cout << "The sum of integers from " << n << " to " << m << " is: " << sum
       << std::endl; // output sum to screen
*/
  // How Gauss would have computed it
  sum = (m + n) * (m - n + 1) / 2;
  std::cout << std::format(
                 "Gauss says that the sum of integers from {} to {} is: {}", n,
                 m, sum)
            << std::endl;
}
// Note: Using std::endl prints out a newline and flushes the output buffer.
// Alternatively, you can use "\n", which does not flush.
// We have used the brace initialization for sum
// Alternatively you may use:
// int sum(0); // old style initialization (but still perfectly valid)
// int sum = 0; // (almost) equivalent to int sum{0};
// auto sum = 0; // Equivalent to int sum=0 with automatic type deduction
// auto sum = 0.0; // Equivalent to double sum=0.0; with automatic type
// deduction for floating points!
// auto sum{0ul}; // sum is an unsigned long integer initialized to 0
