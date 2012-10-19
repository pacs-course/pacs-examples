#include <iostream>       // include a library for input and output 
#include <cstdlib>        // include a library for atoi

int main() {

  using namespace std;
  int n = 10;
  cout << "The value of variable n is: " << n << endl;
  cout << "The location in memory of variable n is: " << &n << endl;

  n = 55;
  cout << "The value of variable n is: " << n << "\n";
  cout << "The location in memory of variable n is: " << &n << endl;

  int* p = &n;
  int m = 200;
  *p = m;
  cout << "The value of variable n is: " << n << endl;
  cout << "The location in memory of variable n is: " << &n << endl;

  for (char i = 'a'; i <= 'z'; i++) cout << i << endl;
  for (int i = 'a'; i <= 'z'; i++) cout << i << endl;

  long double x = 5e30, y = 0.1;
  bool b = x < x + y;
  cout << " x < x + y is:" << b << endl;       // b = false is expected
  cout << " sizeof (long double):" << sizeof(long double) << endl;

  double* tryd = new double (x);
  x = 12.0;
  cout << "tryd is:" << *tryd << endl;       // tryd = 5

  // the compiler should give a warning here
  unsigned int usi = - 1;
  cout << "unsigned int (-1)  = " << usi << endl;

  // but most compilers do not warn here
  int ii = -1;
  //  unsigned int jj = ii;

}
