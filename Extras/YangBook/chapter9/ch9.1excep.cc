#include <cstdlib>
#include <iostream> 
#include <cmath>
#include <complex>
#include <vector>
#include <set>

class ZeroDivisor { };                 // class for zero divisor

class SmallDivisor {
public:
  static const double Small;
  double sd;
  SmallDivisor(double d) { sd = d; }
};

const double SmallDivisor::Small = 1.0e-12;

double f(double d) {                   // fcn that may throw an exception
  if (d == 0.0) throw ZeroDivisor();   // throw an object of ZeroDivisor
  if (fabs(d) < SmallDivisor::Small) 
    throw SmallDivisor(d);             // construct an object from d 
  return 5.0/d;
}

double g(double a) {
  try {                         // try the following code

    double b = f(a);
    std::cout << "No small divisor in f()\n";
    return b;

  } catch (SmallDivisor x) {    // catch exceptions of type SmallDivisor

    std::cout << "Small divisor in f() is " << x.sd << "\n";
    return 10000;               // return 10000 if divisor is small

  } catch(ZeroDivisor) {

    std::cout << "Zero divisor in f()\n";
    return 0;                   // return 0 if divisor is zero

  }
}

int main() {
  double a = g(10);
  std::cout << a << "\n";

  double b = g(1.0e-14);
  std::cout << b << "\n";

  double c = g(0);
  std::cout << c << "\n";
}
