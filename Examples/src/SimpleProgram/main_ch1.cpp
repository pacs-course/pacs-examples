/* A simple program to illustrate some basic features of C++. It adds all
   integers between two given integers and outputs the sum to the screen. */

#include <algorithm> // for std::swap
#include <iostream>  // include a standard library for input/output
int
main()
{
  // using namespace std; // bring std names on the current namespace (avoid doing it)
  using std::swap; // brings swap in the current scope
  using std::cin; // brings std:;cin in the current scope
  int n, m;            // declare n and m to be integer
  // I have not brought cout and endl in the current scope: i use the
  // full qualified name
  std::cout << "Enter two integers:" << std::endl; // output to screen
  cin >> n >> m;                         // input from terminal
  if(n > m)
    swap(n, m); // if n is bigger than m, swap them
  int sum{0};   // sum is initialized to 0
  // a loop, i changes from n to m with increment 1 each time
  for(auto i = n; i <= m; ++i)
    sum += i;
  std::cout << "The sum of integers from " << n << " to " << m << " is: " << sum
       << std::endl; // output sum to screen
}
// Note: Using endl prints out a newline and flushes the output buffer.
// Alternatively you may use "\n".
// We have used the brace initialization for sum
// Alternatively you may use:
// int sum(0); // old style initialization (but still perfectly valid)
// int sum = 0;// Equivalent to int sum{0};
