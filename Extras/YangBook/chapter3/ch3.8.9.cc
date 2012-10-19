#include <iostream>     // include a library for input and output 
#include <cstdlib>       // include a library for atoi

using namespace std;

int gcd(int m, int n) {        // greatest common divisor
  static int callnum = 1;

  cout << "The function gcd() has been called " << callnum++ 
       << " times so far.\n";

  if (n == 0) return m;
  return gcd(n,m%n);
}

int main(int argc, char* argv[]) {

  int i = gcd(2215,335);
  cout << " i = " << i << '\n';
}
