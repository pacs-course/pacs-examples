/* A simple program to illustrate some features of C++. It adds all 
   integers between two given integers and output the sum to the screen.  */

#include <iostream>     // include a library for input and output 
#include <cstdlib>      // include a library for atoi

int main(int argc, char* argv[]) {

  using namespace std;

  if (argc != 3) {
    cout << "two integers must be provided at command line\n";
    exit(1);
  }

  int n = atoi(argv[1]);  // first integer is assigned to n
  int m = atoi(argv[2]);  // second integer is assigned to m
  int low = n, up = n;    // declare low and up and initialize them to n
  if (up < m) up = m;     // if up is smaller than m, assign m to it 
  if (low > m) low = m;   // if low is bigger than m, assign m to it 

  double sum = 0.0;       // sum is of double precision and
                          // initialize it to 0.0
  // a loop, i changes from low to up with increment 1 each time
  for (int i = low; i <= up; i++) { // <= means less than or equal to
    sum += i;                       // equivalent to sum = sum + i;
  }
  cout << "Sum of integers from " << low << " to " << up 
       << " is: " << sum << "\n";   // output the value of sum to screen


  int square(int);               // forward declaration
  int x = square(6);             // call sqaure before its definition
}

int square(int x) {              // definition of a function 
  return x*x;
}

