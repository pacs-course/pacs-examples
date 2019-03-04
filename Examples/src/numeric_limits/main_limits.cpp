#include <iostream>
#include <limits>
#include <string>
#include <cmath> // for isnan and isfinite
//! An example of numeric_limits
/*!  
  It provides a simple example of the content of numeric_limits<>
  class template of the standard library. We give also an example.
 */

// L. Formaggia 2005-2019

//! Prints properties of a float
/*!  
  \tpar F A float type
  \param type a string with the name of the type

  
   Some less obvious stuff
  
   Rounding style. Different architectures may support
   different styles when rounding after an arithmetic operation
   or when converting a floating point to a representation with
   less precision, like a double to a float
   The rounding styles are
    -1  undeterminate
     0  towards zero
     1  to nearest value (IEEE compliant)
     2  towards infinity
     3  towards negative infinity
  
     Round error: it is indeed linked to the rounding style. It is
     the largest possible rounding error (relative0 expressed in
     round-off units u.
     If rounding style is 1 the round error is 0.5, since the maximim
     possible rounding error is 0.5u. 
  
   Signalling and non signalling NaN
  
   Depending on the architecture the occurrence of a NaN may raise
   a floating point exception (but the execution usually IS NOT TERMINATED)
   A quiet NaN is a special representation of a floating point that
   represents a "Not A Number", which however does NOT raise any exception.
   Useful is you need to test if a number is NaN, without reasing the
   exception because of the test! Note however that <cmath> introduces
   the function isnan() that allows you to make the test without
   bothering about these details.
  
   A signalling NaN is a NaN that raises the exception.
  
   Numeric limits allow to test if the architecture supports NaN (and Inf)
   and for NaN whether the signalling and/or the quiet versions are
   implemented. If the system is IEEE compliant you should have both
   quiet and signalling NaN. But you may check it easily with the magic of
   numeric_limits!
*/
template<class F>
void printFloatLimits(std::string const & type)
{
  using namespace std; // simplify life
  cout << "**********************************************"<<endl;
  cout << "*************** "<<type<<" ******************"<<endl;
  cout << "**********************************************"<<endl;
  cout << "largest                = " 
       << numeric_limits<F>::max() << '\n';
  cout << "smallest               = " 
       << numeric_limits<F>::min() << '\n';
  cout << "min exponent in binary = " 
       << numeric_limits<F>::min_exponent << '\n';
  cout << "min exponent in decimal= " 
       << numeric_limits<F>::min_exponent10 << '\n';
  cout << "max exponent in binary = " 
       << numeric_limits<F>::max_exponent << '\n';
  cout << "max exponent in decimal= " 
       << numeric_limits<F>::max_exponent10 << '\n';
  cout << "base of exponent       = " 
       << numeric_limits<F>::radix << '\n';
  cout << "Has infinity:            "<< 
          numeric_limits<F>::has_infinity<<endl;
  cout << "infinity               = " 
       << numeric_limits<F>::infinity() << '\n';
  cout << "Has not a number (quiet):"<< 
          numeric_limits<F>::has_quiet_NaN<<endl;
  cout << "Has not a number (signalling):"<< 
          numeric_limits<F>::has_signaling_NaN<<endl;
  cout << "Not a number           = " 
       << numeric_limits<F>::quiet_NaN() << '\n';
  cout << "machine epsilon        = " 
       << numeric_limits<F>::epsilon() << '\n';
  cout << "round off              = " 
       << numeric_limits<F>::round_error() << '\n';
  cout << "rounding style         = " 
       << numeric_limits<F>::round_style << '\n';
  cout << "# of binary digits in mantissa= " 
       << numeric_limits<F>::digits << '\n';
  cout << "# of decimal digits in mantissa= " 
       << numeric_limits<F>::digits10 << '\n';
  cout<<endl;
}

int main () {
// in global scope only to make life easier
  using namespace std;        

  // write true/false instead of 0/1
  cout.setf(ios::boolalpha);
  cout << "**********************************************"<<endl;
  cout << "***********   NUMERIC  LIMITS ****************"<<endl;
  cout << "**********************************************"<<endl;

  printFloatLimits<float>(" FLOAT ");
  printFloatLimits<double>(" DOUBLE ");
  printFloatLimits<long double>(" LONG DOUBLE ");
  
  cout<<endl;
  cout << "**********************************************"<<endl;
  cout <<"***************    INT   **********************"<<endl;
  cout << "**********************************************"<<endl;
  cout << "largest                = " 
       << numeric_limits<int>::max() << '\n';
  cout << "smallest               = " 
       << numeric_limits<int>::min() << '\n';

  cout<<endl;
  cout << "**********************************************"<<endl;
  cout << "***************    Unsigned INT   ************"<<endl;
  cout << "**********************************************"<<endl;
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
  cout << "**********************************************"<<endl;
  cout << "is char an integral type?: "
       << numeric_limits<char>::is_integer << '\n';
  cout << "**********************************************"<<endl<<endl;

  cout<< "Now a simple test"<<endl;
  // set stream to maximal precision (for a double) and scientific notation
  cout.setf(ios::scientific);
  auto maxprec=numeric_limits<double>::digits10;
  cout.precision(maxprec+1);
  // Test if epsilon is what is meant to be!
  double one(1.0);
  cout<< " 1 + eps  = "<< one + numeric_limits<double>::epsilon()<<endl;
  cout<< " 1 + eps/2= "<< one + numeric_limits<double>::epsilon()/2.0<<endl;
  cout<< " Is 1+eps/2 equal to 1?:"<<
    ((one + numeric_limits<double>::epsilon()/2.0) == one)<<endl;
  cout<<"What happens if an int overflows?\n";
  cout<<"Max integer plus one= "<< numeric_limits<int>::max()+1<<std::endl;
  cout<<"You can test if a number is NaN:\n";
  double y=0./0.;
  cout<<" Is 0./0. a NaN?: "<<std::isnan(y)<<std::endl;
  cout<<"You can test if a number is infnity:\n";
  double z=1./0.;
  cout<<" Is 1./0. equal to Inf?: "<<!std::isfinite(z)<<std::endl;
    
}
