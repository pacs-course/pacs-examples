#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>

int
main()
{
  using std::cin;
  using std::cout;
  using std::endl;
  using std::ios_base;

  cout << "        *****    Changing stream status by flags" << endl << endl;
  // Store current value of flags of cout
  auto oldf = cout.flags();
  // Store current value of precision
  auto oprec = cout.precision();
  // Set scientific format and maximal precision for a double
  // ios_base::scientific is a flag, ios_base::floatfield is a mask
  // The mask is used to ensure that all bit not set in the flag
  // and set in the mask are unset. If
  // If I write just  cout.setf(std::ios_base::scientific) then
  // the bit flag corresponding to ios_base::scientific is set
  // and the other left unchanged. Choose what you need!
  cout.setf(std::ios_base::scientific, std::ios_base::floatfield);
  cout.precision(std::numeric_limits<double>::digits10);
  double pi = std::atan(1.0) * 4.0;
  cout << pi << "Max precision, Scientific format" << endl;
  cout.precision(4);
  cout << pi << " with 4 digits, Scientific format" << endl;
  // I like uppercase E
  // If I use
  // cout.setf(ios_base::uppercase,ios_base::floatfield);
  // I will also clear the ios::scientific flag
  // A note: if I want to set both flags in one single command
  // I can use the bitwise or operator:
  // cout.setf(ios_base::uppercase|ios_base::scientific,ios_base::floatfield);
  //
  cout.setf(ios_base::uppercase);
  cout << pi << " with 4 digits, Scientific format, Uppercase E" << endl;
  auto oldwidth = cout.width();                     // store current width
  cout.setf(ios_base::left, ios_base::adjustfield); // adjust to left
  // Set minumum output width
  cout.precision(3);
  cout.width(25);
  cout << " Width 25 precision 3:" << endl;
  cout << pi << cout.width() << std::endl;
  bool a = true;
  cout << a << " This is a bool" << endl;
  // I want true/false
  cout.setf(ios_base::boolalpha);
  cout << a << " This is again a  bool" << endl;
  // Set minimum width
  cout.precision(8);
  cout.width(25);
  // adjust to internal
  cout.setf(ios_base::internal, ios_base::adjustfield);
  cout.setf(ios_base::dec, ios_base::basefield); // decimal, default
  int j = 234;
  cout << pi << " " << j << " Internal padding" << endl;
  cout.setf(ios_base::left, ios_base::adjustfield);
  cout.width(25);
  cout << pi << " " << j << " Left padding" << endl;
  // Fill with a different character
  char oldfill = cout.fill();
  cout.fill('#');
  cout.setf(ios_base::right, ios_base::adjustfield);
  cout.width(25);
  cout << pi << " " << j << " Right padding with # as padding char" << endl;

  // I want to see the base used
  cout.setf(ios_base::showbase);
  // I want a + in front of positive numbers
  cout.setf(ios_base::showpos);
  int i = 1234;
  cout.setf(ios_base::oct, ios_base::basefield); // octal, base 8
  cout << i << " 1234 in octal base  " << endl;
  cout.setf(ios_base::hex, ios_base::basefield); // hexadeciaml, base =16
  cout << i << " 1234 in exadecimal  " << endl;
  // back to nurmal
  cout.width(oldwidth);
  cout.precision(oprec);
  cout.flags(oldf);
  cout.fill(oldfill);
  cout << " ****************   USING MANIPULATORS" << endl << endl;

  int maxprec = std::numeric_limits<double>::digits10;
  cout << std::setprecision(maxprec) << std::scientific << pi
       << "Max precision, Scientific format" << endl;
  cout << std::setprecision(4) << pi << " with 4 digits, Scientific format"
       << endl;
  // I like uppercase E
  cout << std::uppercase << pi
       << " with 4 digits, Scientific format, Uppercase E" << endl;
  //
  cout << std::setw(25) << std::setprecision(15) << std::left << pi << 123
       << " Width 25 precision 15" << endl;
  cout << a << " This is a bool" << endl;
  cout << std::boolalpha;
  cout << a << " This is again a  bool" << endl;
  //
  cout << std::setw(25) << std::setprecision(8) << std::internal << std::dec;
  cout << pi << " " << j << " Internal padding" << endl;
  cout << std::setw(25) << std::left << pi << " " << j << " Left padding"
       << endl;
  //
  cout << std::showpos << std::setw(25) << std::right << std::setfill('#') << pi
       << " " << j << " Right padding with # as padding char" << endl;
  //
  cout << std::showbase << std::showpos;
  cout << std::oct << i << " 1234 in octal base  " << endl;
  cout << std::hex << i << " 1234 in exadecimal  " << endl;
  // Back to normal
  cout << std::setprecision(oprec) << std::setfill(oldfill)
       << std::setiosflags(oldf);
}
