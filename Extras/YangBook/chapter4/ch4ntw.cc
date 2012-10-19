// chapter 3, section 2, computer problem 1: Newton's Method
// NUMERICAL ANALYSIS: MATHEMATICS OF SCIENTIFIC COMPUTING, SECOND EDITION 
// David Kincaid & Ward Cheney, Brooks/Cole Publishing Co., 1996, 
// ISBN 0-534-3389-5 

#include <iostream>
#include <cmath>
#include <cstdlib>

typedef double (*pfn)(double);               // define a function type

double newton(double xp, pfn f, pfn fd, double delta, double epsn, int mxt){

/**************************************************************************
newton's algorith: finds an approximate root of f(x) = 0
xp:    an initial guess of a root
f:     the function whose root is to be found
fd:    the derivative function of f
mxt:   maximum number of iterations allowed
delta: program stops when distance of two iterates is less than it
epsn:  program stops when residaul is less than it.
**************************************************************************/
  double v = f(xp);                           // fcn value at initial guess

  for (int k = 1; k <= mxt; k++) {            // main iteration loop

    double derv = fd(xp);
    if (derv == 0) { 
      std::cout <<  "Division by zero occurred in function newton().\n";
      std::exit(1);                                // program stops if divisor == 0
    }

    double xnew = xp - v/derv;                // get new iterate
    v = f(xnew);                              // fcn value at new iterate
    if (std::fabs(xnew - xp) < delta || fabs(v) < epsn) return xnew;
    xp = xnew;
  }                                           // end main iteration loop
}   // end newton()

double f(double x) {
  return ((x - 5)*x + 3)*x + 7;
}

double fder(double x) {
  return (3*x  - 10)*x + 3;
}

double g(double x) {
  return 1 - x - std::exp(-x);
}

double gder(double x) {
  return  - 1 + std::exp(-x);
}

double h(double x) {
  return  x - std::tan(x);
}

double hder(double x) {
  return  1 - std::pow(1/std::cos(x),2);
}

main() {
  double root = newton(5, f, fder, 1e-8, 1e-9, 500);
  std::cout <<"Approx root near 5 by newton method is: " << root << '\n';
  std::cout <<"Fcn value at approximate root (residual) is: "<< f(root) << '\n';

  root = newton(1, g, gder, 1e-8, 1e-9, 500);
  std::cout <<"Approx root near 1 by newton method is: " << root << '\n';
  std::cout <<"Fcn value at approximate root (residual) is: "<< g(root) << '\n';

  root = newton(4.5, h, hder, 1e-8, 1e-9, 500);
  std::cout <<"Approx root near 4.5 by newton method is: " << root << '\n';
  std::cout <<"Fcn value at approximate root (residual) is: "<< h(root) << '\n';

  root = newton(7.7, h, hder, 1e-8, 1e-9, 500);
  std::cout <<"Approx root near 7.7 by newton method is: " << root << '\n';
  std::cout <<"Fcn value at approximate root (residual) is: "<< h(root) << '\n';

  root = newton(5, h, hder, 1e-8, 1e-9, 500);
  std::cout <<"Approx root near 5 by newton method is: " << root << '\n';
  std::cout <<"Fcn value at approximate root (residual) is: "<< h(root) << '\n';
}
