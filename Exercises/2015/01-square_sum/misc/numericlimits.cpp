#include <iostream>
#include <limits>
using namespace std;

main() {
    cout << "smallest float                  = " 
         << numeric_limits<float>::min() << endl;
    cout << "largest float                   = " 
         << numeric_limits<float>::max() << endl;
    cout << "min exponent in binary          = " 
         << numeric_limits<float>::min_exponent << endl;
    cout << "min exponent in decimal         = "
         << numeric_limits<float>::min_exponent10 << endl;
    cout << "max exponent in binary          = " 
         << numeric_limits<float>::max_exponent << endl;
    cout << "max exponent in decimal         = "
         << numeric_limits<float>::max_exponent10 << endl;
    cout << "# of binary digits in mantissa  = " 
         << numeric_limits<float>::digits << endl;
    cout << "# of decimal digits in mantissa = "
         << numeric_limits<float>::digits10 << endl;
    cout << "base of exponent in float       = "
         << numeric_limits<float>::radix << endl;
    cout << "infinity in float               = "
         << numeric_limits<float>::infinity() << endl;
    cout << "float epsilon                   = "
         << numeric_limits<float>::epsilon() << endl;
    cout << "float rounding error            = "
         << numeric_limits<float>::round_error() << endl;
    cout << "float rounding style            = "
         << numeric_limits<float>::round_style << endl;
}
