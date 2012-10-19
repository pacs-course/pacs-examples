#include "ch4it.h" 

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

  //cout << "called " << itern << " times \n";
  if (fabs(e) < delta || fabs(w) < epsn || itern++ > maxit) return c; 
  ((u > 0 && w < 0) || (u < 0 && w > 0)) ? (b = c) : (a = c, u = w);
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


double fa(double x) {                         // test problem (b)
  if (x != 0) return 1.0/x - pow(2,x);
  else {
    std::cout << "division by zero occured in function fb().";
    exit(1);
  }
}

double fb(double x) {                         // test problem (c)
  return pow(2,-x) + exp(x) +2*cos(x) - 6;
}

double fc(double x) {                         // test problem (d)
  double denorm = ((2*x-9)*x + 18)*x - 2;
  if (denorm != 0) return (((x+4)*x+ 3)*x +5)/denorm; 
  else {
    std::cout << "division by zero occured in function fd().";
    exit(1);
  }  
}
