#include "tostring.hpp"
#include <iostream>
#include <string>
int
main()
{
  using namespace std;
  using namespace Utilities;
  string a;
  int    n;
  double x;
  n = 10;
  x = 40.5;
  a = string("The value of n is ") + Utilities::toString(n) +
      string("and of x is") + toString(x, ios::scientific);
  cout << a << "\n";
}
