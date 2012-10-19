/* A sample program to illustrate some basic features of C++. It adds all 
   integers between two given integers and output the sum to the screen.      */

#include <iostream>               // include a standard library for input/output
 
int main() {
  using namespace std;

  int n, m;                       // declare n and m to be integers
  cout << "Enter two integers:" << endl;           // output to screen
  cin >> n >> m;                  // input will be assigned to n and m

  if (n > m) {                    // if n is bigger than m, swap them
    int temp = n;                 // declare temp and initialize it 
    n = m;                        // assign value of m to n
    m = temp;                     // assign value of temp to m
  }

  double sum = 0.0;               // sum has double precision, initialized to 0

  // a loop, i changes from n to m with increment 1 each time
  for (int i = n; i <= m; i++) {              // <= means less than or equal to
    sum += i;                                 // sum += i means sum = sum + i;
  }
  cout << "Sum of integers from " << n << " to " << m 
       << " is: " << sum << endl;             // output sum to screen
}

// Note: Using endl prints out a newline and flushes the output buffer. 
