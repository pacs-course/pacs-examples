#include <iostream>
#include <limits>

//! An example of numeric_limits
/*!  
  It provides a simple example of the content of numeric_limits<>
  class template of the standard library. We give also an example.
 */

// L. Formaggia 2005


int main () {
// standard template library
// in global scope
  using namespace std;        

  // write true/flase instead of 0/1
  cout.setf(ios::boolalpha);
  

  cout << "**********************************************"<<endl;
  cout << "***********   NUMERIC  LIMITS ****************"<<endl;
  cout << "**********************************************"<<endl;
  cout << "***************    FLOATS   ******************"<<endl;
  cout << "largest                = " 
       << numeric_limits<float>::max() << '\n';
  cout << "smallest               = " 
       << numeric_limits<float>::min() << '\n';
  cout << "min exponent in binary = " 
       << numeric_limits<float>::min_exponent << '\n';
  cout << "min exponent in decimal= " 
       << numeric_limits<float>::min_exponent10 << '\n';
  cout << "max exponent in binary = " 
       << numeric_limits<float>::max_exponent << '\n';
  cout << "max exponent in decimal= " 
       << numeric_limits<float>::max_exponent10 << '\n';
  cout << "base of exponent       = " 
       << numeric_limits<float>::radix << '\n';
  cout << "Has infinity:            "<< 
          numeric_limits<float>::has_infinity<<endl;
  cout << "infinity               = " 
       << numeric_limits<float>::infinity() << '\n';
  cout << "Has not a number (quiet):"<< 
          numeric_limits<float>::has_quiet_NaN<<endl;
  cout << "Not a number           = " 
       << numeric_limits<float>::quiet_NaN() << '\n';
  cout << "machine epsilon        = " 
       << numeric_limits<float>::epsilon() << '\n';
  cout << "round off              = " 
       << numeric_limits<float>::round_error() << '\n';
  cout << "rounding style         = " 
       << numeric_limits<float>::round_style << '\n';
  cout << "# of binary digits in mantissa= " 
       << numeric_limits<float>::digits << '\n';
  cout << "# of decimal digits in mantissa= " 
       << numeric_limits<float>::digits10 << '\n';
  cout<<endl;
  cout << "**********************************************"<<endl;
  cout <<"***************    DOUBLE   ******************"<<endl;
  cout << "largest                = " 
       << numeric_limits<double>::max() << '\n';
  cout << "smallest               = " 
       << numeric_limits<double>::min() << '\n';
  cout << "min exponent in binary = " 
       << numeric_limits<double>::min_exponent << '\n';
  cout << "min exponent in decimal= " 
       << numeric_limits<double>::min_exponent10 << '\n';
  cout << "max exponent in binary = " 
       << numeric_limits<double>::max_exponent << '\n';
  cout << "max exponent in decimal= " 
       << numeric_limits<double>::max_exponent10 << '\n';
  cout << "base of exponent       = " 
       << numeric_limits<double>::radix << '\n';
  cout << "infinity               = " 
       << numeric_limits<double>::infinity() << '\n';
  cout << "machine epsilon        = " 
       << numeric_limits<double>::epsilon() << '\n';
  cout << "round off              = " 
       << numeric_limits<double>::round_error() << '\n';
  cout << "rounding style         = " 
       << numeric_limits<double>::round_style << '\n';
  cout << "# of binary digits in mantissa= " 
       << numeric_limits<double>::digits << '\n';
  cout << "# of decimal digits in mantissa= " 
       << numeric_limits<double>::digits10 << '\n';
  
  cout<<endl;
  cout << "**********************************************"<<endl;
  cout <<"***************   LONG DOUBLE   ******************"<<endl;
  cout << "largest                = " 
       << numeric_limits<long double>::max() << '\n';
  cout << "smallest               = " 
       << numeric_limits<long double>::min() << '\n';
  cout << "min exponent in binary = " 
       << numeric_limits<long double>::min_exponent << '\n';
  cout << "min exponent in decimal= " 
       << numeric_limits<long double>::min_exponent10 << '\n';
  cout << "max exponent in binary = " 
       << numeric_limits<long double>::max_exponent << '\n';
  cout << "max exponent in decimal= " 
       << numeric_limits<long double>::max_exponent10 << '\n';
  cout << "base of exponent       = " 
       << numeric_limits<long double>::radix << '\n';
  cout << "infinity               = " 
       << numeric_limits<long double>::infinity() << '\n';
  cout << "machine epsilon        = " 
       << numeric_limits<long double>::epsilon() << '\n';
  cout << "round off              = " 
       << numeric_limits<long double>::round_error() << '\n';
  cout << "rounding style         = " 
       << numeric_limits<long double>::round_style << '\n';
  cout << "# of binary digits in mantissa= " 
       << numeric_limits<long double>::digits << '\n';
  cout << "# of decimal digits in mantissa= " 
       << numeric_limits<long double>::digits10 << '\n';
  
  cout<<endl;
  cout << "**********************************************"<<endl;
  cout <<"***************    INT   **********************"<<endl;
  cout << "largest                = " 
       << numeric_limits<int>::max() << '\n';
  cout << "smallest               = " 
       << numeric_limits<int>::min() << '\n';

  cout<<endl;
  cout << "**********************************************"<<endl;
  cout << "***************    Unsigned INT   ************"<<endl;
  cout << "largest                = " 
       << numeric_limits<unsigned int>::max() << '\n';
  cout << "smallest               = " 
       << numeric_limits<unsigned int>::min() << '\n';

  cout << endl<<"***************    long INT   *********"<<endl;
  cout << "largest                = " 
       << numeric_limits<long int>::max() << '\n';
  cout << "smallest               = " 
       << numeric_limits<long int>::min() << '\n';

  cout<<endl;
  cout << "**********************************************"<<endl;
  cout << "***************    CHAR   ********************"<<endl;
  cout << "is char an integral type?: "
       << numeric_limits<char>::is_integer << '\n';

  cout<< "Now a simple test"<<endl;
  // set stream to maximal precision and scientific notation
  cout.setf(ios::scientific);
  auto maxprec=numeric_limits<double>::digits10;
  cout.precision(maxprec+1);
  // Test if epsilon is what is meant to be!
  double one(1.0);
  cout<< " 1 + eps  = "<< one + numeric_limits<double>::epsilon()<<endl;
  cout<< " 1 + eps/2= "<< one + numeric_limits<double>::epsilon()/2.0<<endl;
  cout<< " Is 1+eps/2 equal to 1?:"<<
    ((one + numeric_limits<double>::epsilon()/2.0) == one)<<endl; 
}
