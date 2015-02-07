/* A sample program to illustrate some basic features of C++. It adds all
   integers between two given integers and outputs the sum to the screen.      */

#include <iostream> // include a standard library for input/output
#include <algorithm> // for std::swap
int main() {
  using namespace std; // bring std names on the current namespace
  int n, m;            // declare n and m to be integers
  cout << "Enter two integers:" << endl;// output to screen
  cin >> n >> m; // input from terminal
  if (n > m)swap(n,m);  // if n is bigger than m, swap them
  int sum(0);               // sum is initialized to 0
  // a loop, i changes from n to m with increment 1 each time
  for (auto i = n; i <= m; ++i) sum +=i;                 
  cout << "Sum of integers from " << n << " to " << m
       << " is: " << sum << endl; // output sum to screen
}

// Note: Using endl prints out a newline and flushes the output buffer.
