#include <iostream>
#include "iterativeMethod.hpp"
#include <limits>

template<typename real>
real f(real x) 
{
  return x*x - .5;
};

template<typename real>
real df(real x) 
{
  return 2 * x;
};


using namespace std;

int main() {

  /// Solve the problem using 64 bit doubles
  Robust<double> 
    r_double (numeric_limits<double>::epsilon () * 4, 100, 
              IterativeMethod<double>::INCREMENT, 
              2.e5);

  double result_double = r_double.apply (0., 1., f, df);
  cout << "using " << sizeof (double) 
       << " byte floating point representation" << endl;
  cout << result_double << endl;
  cout << r_double << endl << endl;

  /// Solve the problem using 32 bit doubles
  Robust<float>
    r_float (numeric_limits<float>::epsilon () * 4, 100, 
             IterativeMethod<float>::INCREMENT, 
             2.e5);

  float result_float = r_float.apply (0., 1., f, df);
  cout << "using " << sizeof (float) 
       << " byte floating point representation" << endl;
  cout << result_float << endl;
  cout << r_float << endl << endl;

  /// Solve the problem using 128 bit doubles
  Robust<long double> 
    r_ldouble (numeric_limits<long double>::epsilon () * 4, 100, 
               IterativeMethod<long double>::INCREMENT, 
               2.e5);

  long double result_ldouble = r_ldouble.apply (0., 1., f, df);
  cout << "using " << sizeof (long double) 
       << " byte floating point representation" << endl;
  cout << result_ldouble << endl;
  cout << r_ldouble << endl << endl;

  return 0;
}
