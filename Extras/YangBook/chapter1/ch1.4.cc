#include <iostream>
#include <limits>

using namespace std;        // namespace is discussed later

int main () {
  cout << "largest float = " 
       << numeric_limits<float>::max() << '\n';
  cout << "smallest float = " 
       << numeric_limits<float>::min() << '\n';
  cout << "min exponent in binary = " 
       << numeric_limits<float>::min_exponent << '\n';
  cout << "min exponent in decimal = " 
       << numeric_limits<float>::min_exponent10 << '\n';
  cout << "max exponent in binary = " 
       << numeric_limits<float>::max_exponent << '\n';
  cout << "max exponent in decimal = " 
       << numeric_limits<float>::max_exponent10 << '\n';
  cout << "# of binary digits in mantissa: " 
       << numeric_limits<float>::digits << '\n';
  cout << "# of decimal digits in mantissa: " 
       << numeric_limits<float>::digits10 << '\n';
  cout << "base of exponent in float: " 
       << numeric_limits<float>::radix << '\n';
  cout << "infinity in float: " 
       << numeric_limits<float>::infinity() << '\n';
  cout << "float epsilon = " 
       << numeric_limits<float>::epsilon() << '\n';
  cout << "float rounding error = " 
       << numeric_limits<float>::round_error() << '\n';
  cout << "float rounding style = " 
       << numeric_limits<float>::round_style << '\n';

  cout << "smallest double = " 
       << numeric_limits<double>::min() << '\n';
  cout << "largest long double = " 
       << numeric_limits<long double>::max() << '\n';
  cout << "double epsilon = " 
       << numeric_limits<double>::epsilon() << '\n';
  cout << "long double epsilon = " 
       << numeric_limits<long double>::epsilon() << '\n';

  cout << "number of digits in char: " 
       << numeric_limits<char>::digits << '\n';
  cout << "char is singed or not: "
       << numeric_limits<char>::is_signed << '\n';
  cout << "smallest char: "
       << numeric_limits<char>::min() << '\n';
  cout << "biggest char: "
       << numeric_limits<char>::max() << '\n';
  cout << "is char an integral type: "
       << numeric_limits<char>::is_integer << '\n';
}
