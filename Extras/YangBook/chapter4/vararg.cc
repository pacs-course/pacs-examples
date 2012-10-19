/*
 * Sampe program illustrating functions of a variable number of arguments.
 */

#include <cstdarg>
#include <iostream>

double sum(int num, ...);

main() {

  double d   = sum(3, 0.5, 0.25, 0.125);
  double dd  = sum(4, 0.5, 0.25, 0.125, 0.0625);
  double ddd = sum(5, 0.5, 0.25, 0.125, 0.0625, 0.03125);

  std::cout << " d  = "  << d   << std::endl;
  std::cout << " dd  = " << dd  << std::endl;
  std::cout << " ddd = " << ddd << std::endl;
}

// num represents the number of arguments
double sum(int num, ...) {

  using namespace std;

  va_list argPtr;
  va_start(argPtr, num);  // initialize argPrt. num is the last known argument

  double sum = 0;

  for( ; num; num--) {
    sum += va_arg(argPtr, double);   // argument of type double is returned
  }

  va_end(argPtr);          // deallocate stack pointed to by argPtr
  return sum;
}
