#include <iostream>               // include a standard library for input/output
#include <math.h>                 // include math library for sine function
 
using namespace std;

int main() {

  int i = 321;           // stored in sizeof(int) bytes
  short ii = 321;        // stored in sizeof(short) bytes
  long iii = i;          // implicit conversion from int to long 
  iii = long(i);         // explicit conversion from int to long
  iii = i + ii;          // implicit conversion, same as iii =long(i) + long(ii)
  iii = 123456789;       // a big integer
  ii = short(iii);       // conversion from long to short, truncation may occur

  cout << "ii = " << ii << endl;       // on my machine, the output of ii is
  cout << "short int = " << sizeof(short) << endl;
  cout << "int = " << sizeof(int) << endl;
  cout << "long int = " << sizeof(long) << endl;

  char cc = '\n';
  cout << "\\n = " << int(cc) << endl;

  cout << "He said: \"I know how to output double q marks\".\n";
  cout << "She asked : \"Do you know how to output newline \\n \? \"\n";

  double sum = 0;                 // sum has double precision, initialized to 0
  for (double d = 1.1; d <= 9.9; d += 0.2) sum += sin(d);                  
  cout << "The sum is: " << sum << endl;                 // output sum to screen

  cout << "sizeof(\"II\") = " << sizeof("II") << endl;     
  cout << "sizeof(\"I\") = " << sizeof("I") << endl;     
  cout << "sizeof('I') = " << sizeof('I') << endl;     
}
