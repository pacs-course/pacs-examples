#include <iostream>   

using namespace std;

int main() {

  long f0 = 1;                                      // previous Fibonacci number
  long f1 = 1;                                      // current Fibonacci number

  cout.width(2);
  cout << "n";
  cout.width(30);
  cout << "Finbonacci number";
  cout.width(30);
  cout << "Finbonacci quotient" << "\n";
  cout.precision(20);                               // for Fibonacci quotients

  for (int i = 2; i <= 40; i++) {
    cout.width(2);
    cout << i;
    cout.width(30);
    cout << f1;                                     // output Fibonacci number
    cout.width(30);                              
    cout << (long double)f1/ f0 << "\n";            // output Fibonacci quotient

    long tmp = f1;
    f1 += f0;                                       // update values
    f0 = tmp;                                 
  }

}
