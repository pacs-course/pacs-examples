// chapter 3, section 1, computer problem 1: Bisection Method
// NUMERICAL ANALYSIS: MATHEMATICS OF SCIENTIFIC COMPUTING, SECOND EDITION 
// David Kincaid & Ward Cheney, Brooks/Cole Publishing Co., 1996, 
// ISBN 0-534-3389-5 

#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;


/*
typedef double (*pfn)(double);                   // define a function type
double bisection(double a, double b, pfn f, int maxit,    
		 double delta, double epsn) {
*/

double bisctn0(double a, double b, double (*f)(double), 
	       double delta, double epsn) {
  double c = (a + b)*0.5;                        // middle point
  if (fabs(b-a)*0.5 < delta || fabs(f(c)) < epsn) return c; 
  (f(a)*f(c) < 0) ? b = c : a = c;
  return bisctn0(a, b, f, delta, epsn);
}

// disadvantages of bisctn0():
// c can be computed in more robust way
// three function evaluations f(c), f(a), f(c) at each recursion
// f(a)*f(c) can cause underflow or overflow while f(a) and f(c) within range

double bisctn1(double a, double b, double (*f)(double), double u, 
	       double delta, double epsn) {
  double e = (b - a)*0.5;                        // shrink interval size
  double c = a + e;                              // middle point
  double w = f(c);                               // fcn value at middle point

  if (fabs(e) < delta || fabs(w) < epsn) return c; 
  ((u > 0 && w < 0) || (u < 0 && w > 0)) ? (b = c) : (a = c, u = w);
/*
  if ((u > 0 && w < 0) || (u < 0 && w > 0)) {    // if u, w differ in sign
    b = c;  
  } else {                                       // if u, w have same sign
    a = c; u = w;
  }
*/
  return bisctn1(a, b, f, u, delta, epsn);
}

// disadvantages of bisctn1():
// may go into an infinite loop

double bisctn2(double a, double b, double (*f)(double), double u, 
	       double delta, double epsn, int maxit) {
  static int itern = 1;
  double e = (b - a)*0.5;                        // shrink interval size
  double c = a + e;                              // middle point
  double w = f(c);                               // fcn value at middle point

  if (fabs(e) < delta || fabs(w) < epsn || itern++ > maxit) return c; 
  ((u > 0 && w < 0) || (u < 0 && w > 0)) ? (b = c) : (a = c, u = w);

  //cout << "called " << itern << " times \n";
  return bisctn2(a, b, f, u, delta, epsn, maxit);
}

// disadvantages of bisctn2():
// static variable itern persists through all calls to bisctn2().
// fatal error: subsequent calls may terminate prematurely.

double bisectionr(double a, double b, double (*f)(double), double u, 
                  double delta, double epsn, int maxit, int& itern) {
/****************************************************************************
 bisection algm: recursive version 
                 returns an approximate root of a fcn in a given interval
 a, b:  end points of given interval in which a root is to be found
 f:     given function that is defined in interval [a,b] or [b,a].
 u:     = f(a)
 delta: root tolerance, return when updated interval is narrower than it 
 epsn:  residual tolerance, return when residual is smaller than it
 maxit: maximum number of iterations allowed
 itern: iteration count, it must be initialized to 1.
 *****************************************************************************/

  double e = (b - a)*0.5;                        // shrink interval size
  double c = a + e;                              // middle point
  double w = f(c);                               // fcn value at middle point

  if (fabs(e) < delta || fabs(w) < epsn || itern++ > maxit) return c; 
  ((u > 0 && w < 0) || (u < 0 && w > 0)) ? (b = c) : (a = c, u = w);
  return bisectionr(a, b, f, u, delta, epsn, maxit, itern);
}


double bisection(double a, double b, double (*f)(double),
		 double delta, double epsn, int maxit) {
/****************************************************************************
 bisection algm: non-recursive version 
                 returns an approximate root of a fcn in a given interval
 a, b:  end points of given interval in which a root is to be found
 f:     given function that is defined in interval [a,b] or [b,a].
 maxit: maximum number of iterations allowed
 delta: root tolerance, return when updated interval is narrower than it 
 epsn:  residual tolerance, return when residual is smaller than it
 ****************************************************************************/

  double u = f(a);                            // fcn value at left pt
  double e = b - a;                           // interval length

  for (int k = 1; k <= maxit; k++) {          // main iteration loop
    e *= 0.5;                                 // shrink interval by half
    double c = a + e;                         // update middle pt     
    double w = f(c);                          // fcn value at middle pt     
    if (fabs(e) < delta || fabs(w) < epsn) return c;
    ((u > 0 && w < 0) || (u < 0 && w > 0)) ? (b = c) : (a = c, u = w);
  }                       
}   // end bisection()


double fa(double x) {                         // test problem (a)
  if (x != 0) return 1.0/x - tan(x);
  else {
    cout << "division by zero occured in function fa().";
    exit(1);
  }
}

double fb(double x) {                         // test problem (b)
  if (x != 0) return 1.0/x - pow(2,x);
  else {
    cout << "division by zero occured in function fb().";
    exit(1);
  }
}

double fc(double x) {                         // test problem (c)
  return pow(2,-x) + exp(x) +2*cos(x) - 6;
}

double fd(double x) {                         // test problem (d)
  double denorm = ((2*x-9)*x + 18)*x - 2;
  if (denorm != 0) return (((x+4)*x+ 3)*x +5)/denorm; 
  else {
    cout << "division by zero occured in function fd().";
    exit(1);
  }  
}

int main() {
  const int max = 50;
  const double delta = 1.0e-8;
  const double epsn = 1.0e-9;
  int itern = 1;

  double root = bisection(1.0e-2, 3.15/2 - 1.0e-2, fa, delta, epsn, max);
//  root = bisection(3.15/2 - 1.0e-2, 0.01, fa, max, delta, epsn);
//  cout << "*********  output for computer problem 3.1(a) ***** \n";
  cout << "Approximate root of fa() by bisection method is:" << root << '\n';
  cout << "Fcn value at approximate root (residual) is:" << fa(root) << '\n';
  cout << "Approximate root by bisctn0() is:" 
       << bisctn0(1.0e-2, 3.14/2 - 1.0e-2, fa, delta, epsn) << '\n';
//       << bisctn0(3.14/2 - 1.0e-2, 0.01, fa, delta, epsn) << '\n';
  cout << "Approximate root by bisctn1() is:" 
       << bisctn1(3.14/2 - 1.0e-2, 1.0e-2, fa, fa(3.14/2-1.0e-2), 
                  delta, epsn) << '\n';
  cout << "Approximate root by bisctn2() is:" 
       << bisctn2(3.14/2 - 1.0e-2, 1.0e-2, fa, fa(3.14/2-1.0e-2),
                  delta, epsn, max) << '\n';
  itern = 1;
  cout << "Approximate root by bisectionr() is:" 
       << bisectionr(3.14/2 - 1.0e-2, 1.0e-2, fa, fa(3.14/2-1.0e-2),
                     delta, epsn, max, itern) << '\n';
  cout << "Number of iterations used in bisectionr() = " << itern << '\n'; 

  root = bisection(1.0e-2, 1, fb, delta, epsn, max);
  root = bisection(1, 1.0e-2, fb, delta, epsn, max);
//  cout << "\n\n*********  output for computer problem 3.1(b) ***** \n";
  cout << "Approximate root of fb() by bisection method is:" << root << '\n';
  cout << "Fcn value at approximate root (residual) is:" << fb(root) << '\n';
  cout << "Approximate root by bisctn0() is:" 
       << bisctn0(1.0e-2, 1, fb, delta, epsn) << '\n';
//       << bisctn0(1,1.0e-2, fb, delta, epsn) << '\n';
  cout << "Approximate root by bisctn1() is:" 
       << bisctn1(1.0e-2, 1, fb, fb(1.0e-2), delta, epsn) << '\n';
  cout << "Approximate root by bisctn2() is:" 
       << bisctn2(1.0e-2, 1, fb, fb(1.0e-2), delta, epsn, max) << '\n';
  itern = 1;
  cout << "Approximate root by bisectionr() is:" 
       << bisectionr(1.0e-2, 1, fb, fb(1.0e-2), delta, epsn, 
                     max, itern) << '\n';
  cout << "Number of iterations used in bisectionr() = " << itern << '\n'; 

  root = bisection(1, 3, fc, delta, epsn, max);
  root = bisection(3, 1, fc, delta, epsn, max);
//  cout << "\n\n*********  output for computer problem 3.1(c) ***** \n";
  cout << "Approximate root of fc() by bisection method is:" << root << '\n';
  cout << "Fcn value at approximate root (residual) is:" << fc(root) << '\n';
  cout << "Approximate root by bisctn0() is:" 
       << bisctn0(1, 3, fc, delta, epsn) << '\n';
//       << bisctn0(3, 1, fc, delta, epsn) << '\n';
  cout << "Approximate root by bisctn1() is:" 
       << bisctn1(1, 3, fc, fc(1), delta, epsn) << '\n';
  cout << "Approximate root by bisctn2() is:" 
       << bisctn2(1, 3, fc, fc(1), delta, epsn, max) << '\n';
  itern = 1;
  cout << "Approximate root by bisectionr() is:" 
       << bisectionr(1, 3, fc, fc(1), delta, epsn, max, itern) << '\n';
  cout << "Number of iterations used in bisectionr() = " << itern << '\n'; 

  root = bisection(0, 4, fd, delta, epsn, max);
  root = bisection(4, 0, fd, delta, epsn, max);
//  cout << "\n\n*********  output for computer problem 3.1(d) ***** \n";
  cout << "Approximate root of fd() by bisection method is:" << root << '\n';
  cout << "Fcn value at approximate root (residual) is:" << fb(root) << '\n';
  cout << "Approximate root by bisctn0 is:" 
       << bisctn0(0, 4, fd, delta, epsn) << '\n';
//       << bisctn0(4, 0, fd, delta, epsn) << '\n';
  cout << "Approximate root by bisctn1() is:" 
       << bisctn1(0, 4, fd, fd(0), delta, epsn) << '\n';
  cout << "Approximate root by bisctn2() is:" 
       << bisctn2(0, 4, fd, fd(0), delta, epsn, max) << '\n';
  itern = 1;
  cout << "Approximate root by bisectionr() is:" 
       << bisectionr(0, 4, fd, fd(0), delta, epsn, max,itern) << '\n';
  cout << "Numer of iterations used in bisectionr() = " << itern << '\n'; 
}
