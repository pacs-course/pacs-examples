#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;

int main() {

{ // integer output
  int i = 123456;
  cout.setf(ios_base::dec, ios_base::basefield);    // decimal, default
  cout << i << "  " << i << "\n";

  // or decimal and octal
  cout << dec << i << "  " << oct << i << "\n";

  cout.setf(ios_base::showbase);            
  cout.setf(ios_base::showpos);              

  cout.setf(ios_base::oct, ios_base::basefield);    // octal, base 8
  cout << i << "  " << i << "\n";
  cout.setf(ios_base::hex, ios_base::basefield);    // hexadeciaml, base =16
  cout << i << "  " << i << "\n";

  cout.unsetf(ios_base::showbase | ios_base::showpos);   
  cout.setf(ios_base::dec, ios_base::basefield);    // decimal, default

  cout << i << "  " << i << "\n";
  cout.setf(ios_base::oct, ios_base::basefield);    // octal, base 8
  cout << i << "  " << i << "\n";
  cout.setf(ios_base::hex, ios_base::basefield);    // hexadeciaml, base =16
  cout << i << "  " << i << "\n";
  cout.setf(ios_base::dec, ios::basefield);           // reset back to default
  cout << i << "  " << i << "\n";
}

{
  double d = - 12345.678987654321;
  cout.setf(ios_base::scientific, ios_base::floatfield);   // scientific format
  cout.setf(ios_base::uppercase);
  cout.width(25);
  cout.precision(15);
  cout.setf(ios_base::left, ios_base::adjustfield);        // adjust to left
  cout << d << "\n";
  cout.width(25);
  cout.precision(8);
  cout.setf(ios_base::internal, ios_base::adjustfield);    // adjust to internal 
  cout.fill('#');
  cout << d << "\n";
}

}
