/* A sample program to illustrate some basic features of C++. It adds all 
   integers between two given integers and output the sum to the screen.      */

#include <iostream>               // include a standard library for input/output
 
using namespace std;

int main() {
  int n, m;                       // declare n and m to be integers
  cout << "Enter two integers: \n";           // output to screen
  cin >> n >> m;                  // input will be assigned to n and m

  if (n > m) {                    // if n is bigger than m, swap them
    int temp = n;                 // declare temp and initialize it 
    n = m;                        // assign value of m to n
    m = temp;                     // assign value of temp to m
  }

  double sum = 0.0;               // sum has double precision, initialized to 0

  // a loop, i changes from n to m with increment 1 each time
  for (int i = n; i <= m; i++) {              // <= means less than or equal to
    sum += i*i;                                 // sum += i means sum = sum + i;
  }
  cout << "Sum of squares of integers from " << n << " to " << m 
       << " is: " << sum << '\n';             // output sum to screen
  cout << "check = " << (m*(m+1)*(2*m+1) - (n-1)*n*(2*n-1))/6 << "\n";

  int sumint = 0;
  for (int i = n; i <= m; i++) sumint += i*i;               
  cout << "Sum of squares of integers from " << n << " to " << m 
       << " is (as an int): " << sumint << '\n';  
}
