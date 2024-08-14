/*
Copyright 2020 <Luca Formaggia, Carlo de Falco>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <cmath> // for isnan and isfinite
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <map>
// numbers has been introduced in C++20
#if __cplusplus >= 202002L
#include <numbers>
#endif
/*!
 * \file numeric_limits.cpp
 *  An example of numeric_limits
 *  It provides a simple example of the content of numeric_limits<>
 *  class template of the standard library. We give also an example of
 *  the properties of eps and of integer overflow.
 */

/*!
  \brief A template function to print properties of a floating point type
  \tpar F A float type
  \param type a string with the name of the type

  \details Some less obvious stuff

   <b>Rounding style</b>. Different architectures may support
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
     the largest possible rounding error (expressed in
     round-off units u).
     If rounding style is 1 the round error is 0.5, since the maximim
     possible rounding error is 0.5u.

   <b> Signalling and non signalling NaN.</b> Depending on the
   architecture the occurrence of a NaN may raise a floating point
   exception (but the execution usually IS NOT TERMINATED). A quiet NaN
   is a special representation of a floating point that represents a
   "Not A Number", which however does NOT raise any exception.  Useful
   if you want to test if a number is NaN, without reasing the
   exception because of the test. Note however that <cmath> introduces
   the function isnan() that allows you to make the test without
   bothering about these details.

   A signalling NaN is a NaN that raises the exception.

   Numeric limits allow to test if the architecture supports NaN (and Inf)
   and for NaN whether the signalling and/or the quiet versions are
   implemented. If the system is IEEE compliant you should have both
   quiet and signalling NaN. But you may check it easily with the magic of
   numeric_limits.
*/
template <class F>
void
printFloatLimits(std::string const &type)
{
  using namespace std; // to simplify life
  // This map is to have a more significant name for the rounding style
  map<std::float_round_style,std::string> roundStyleName {
    {std::float_round_style::round_indeterminate, "round_indeterminate"},
    {std::float_round_style::round_toward_zero, "round_toward_zero"},
    {std::float_round_style::round_to_nearest, "round_to_nearest"},
    {std::float_round_style::round_toward_infinity, "round_toward_infinity"},
    {std::float_round_style::round_toward_neg_infinity, "round_toward_neg_infinity"}
  };
  cout << "**********************************************" << endl;
  cout << "*************** " << type << " ******************" << endl;
  cout << "**********************************************" << endl;
  cout << "largest                = " << numeric_limits<F>::max() << '\n';
  cout << "smallest               = " << numeric_limits<F>::min() << '\n';
  cout << "min exponent in base 2 = " << numeric_limits<F>::min_exponent
       << '\n';
  cout << "min exponent in base 10= " << numeric_limits<F>::min_exponent10
       << '\n';
  cout << "max exponent in base 2 = " << numeric_limits<F>::max_exponent
       << '\n';
  cout << "max exponent in base 10= " << numeric_limits<F>::max_exponent10
       << '\n';
  cout << "base of exponent       = " << numeric_limits<F>::radix << '\n';
  cout << "Has infinity:            " << numeric_limits<F>::has_infinity
       << endl;
  cout << "infinity               = " << numeric_limits<F>::infinity() << '\n';
  cout << "Has not a number (quiet):" << numeric_limits<F>::has_quiet_NaN
       << endl;
  cout << "Has not a number (signalling):"
       << numeric_limits<F>::has_signaling_NaN << endl;
  cout << "Not a number           = " << numeric_limits<F>::quiet_NaN() << '\n';
  cout << "machine epsilon        = " << numeric_limits<F>::epsilon() << '\n';
  cout << "round off              = " << numeric_limits<F>::round_error()
       << '\n';
  cout << "rounding style         = " << roundStyleName[numeric_limits<F>::round_style] << '\n';
  cout << "# of binary digits in mantissa= " << numeric_limits<F>::digits
       << '\n';
  cout << "# of decimal digits in mantissa= " << numeric_limits<F>::digits10
       << '\n';
  cout << endl;
  /* Note
  rounding_style is an enum class with the following values
     enum class float_round_style {
     round_indeterminate = -1,
     round_toward_zero = 0,
     round_to_nearest = 1,
     round_toward_infinity = 2,
     round_toward_neg_infinity = 3
     };

     round_off is the largest possible rounding error (expressed in
     round-off units u). If rounding style is 1 the round error is 0.5,
     */
}

int
main()
{
  // in global scope only to make life easier
  using namespace std;

  // write true/false instead of 0/1
  cout.setf(ios::boolalpha);
  cout << "**********************************************" << endl;
  cout << "***********   NUMERIC  LIMITS ****************" << endl;
  cout << "**********************************************" << endl;

  printFloatLimits<float>(" FLOAT ");
  printFloatLimits<double>(" DOUBLE ");
  printFloatLimits<long double>(" LONG DOUBLE ");

  cout << endl;
  cout << "**********************************************" << endl;
  cout << "***************    INT   **********************" << endl;
  cout << "**********************************************" << endl;
  cout << "largest                = " << numeric_limits<int>::max() << '\n';
  cout << "smallest               = " << numeric_limits<int>::min() << '\n';

  cout << endl;
  cout << "**********************************************" << endl;
  cout << "***************    Unsigned INT   ************" << endl;
  cout << "**********************************************" << endl;
  cout << "largest                = " << numeric_limits<unsigned int>::max()
       << '\n';
  cout << "smallest               = " << numeric_limits<unsigned int>::min()
       << '\n';

  cout << "**********************************************" << endl;
  cout << endl << "***************    long int   *********" << endl;
  cout << "**********************************************" << endl;
  cout << "largest                = " << numeric_limits<long int>::max()
       << '\n';
  cout << "smallest               = " << numeric_limits<long int>::min()
       << '\n';
  cout << "**********************************************" << endl;
  cout << endl << "***************    long unsigned   *********" << endl;
  cout << "**********************************************" << endl;
  cout << "largest                = " << numeric_limits<long unsigned int>::max()
       << '\n';
  cout << "smallest               = " << numeric_limits<long unsigned int>::min()
       << '\n';

  cout << endl;
  cout << "**********************************************" << endl;
  cout << "***************    CHAR   ********************" << endl;
  cout << "**********************************************" << endl;
  cout << "is char an integral type?: " << numeric_limits<char>::is_integer
       << '\n';
  cout << "**********************************************" << endl << endl;

  cout << "We set cout to maximal precision for a double and use scientific "
          "notation"
       << endl;
  cout.setf(ios::scientific);
  auto maxprec = numeric_limits<double>::digits10;
  cout.precision(maxprec);
  cout << " We test if epsilon is what it is meant to be!\n";
  double one(1.0);
  cout << " 1 + eps  = " << one + numeric_limits<double>::epsilon() << endl;
  cout << " 1 + eps/2= " << one + numeric_limits<double>::epsilon() / 2.0
       << endl;
  cout << " Is 1+eps/2 equal to 1?:"
       << ((one + numeric_limits<double>::epsilon() / 2.0) == one) << endl;
  cout << "What happens if an int overflows?\n";
  cout << "Max integer plus one= " << numeric_limits<int>::max() + 1
       << std::endl;
  cout << "What happens if an unsigned int underflows?\n";
  unsigned int zero = 0u;
  unsigned int under = zero - 1u;
  cout << "0u-1u as unsigned integers= " << under << std::endl;
  cout << "You can test if a number is NaN:\n";
  double y = 0. / 0.;
  cout << " Is 0./0. a NaN?: " << std::isnan(y) << std::endl;
  cout << "You can test if a number is infinity:\n";
  double z = 1. / 0.;
  cout << " Is 1./0. equal to Inf?: " << !std::isfinite(z) << std::endl;

#if __cplusplus >= 202002L
  cout << "\n***    Now some c++20 goodies. Predefined constants ***\n";
  using namespace std::numbers;
  std::cout << "Pi (double)        " << pi << std::endl;
  std::cout << "e  (double)        " << e << std::endl;
  std::cout << "sqr(2)(double)     " << sqrt2 << std::endl;

  maxprec = numeric_limits<long double>::digits10;
  cout.precision(maxprec);
  std::cout << "Pi (long double)   " << pi_v<long double> << std::endl;
  std::cout << "e  (long double)   " << e_v<long double> << std::endl;
  std::cout << "sqr(2)(long double)" << sqrt2_v<long double> << std::endl;
  maxprec = numeric_limits<float>::digits10;
  cout.precision(maxprec);
  std::cout << "Pi (float)         " << pi_v<float> << std::endl;
  std::cout << "e  (float)         " << e_v<float> << std::endl;
  std::cout << "sqr(2)(float)      " << sqrt2_v<float> << std::endl;
#endif
}
