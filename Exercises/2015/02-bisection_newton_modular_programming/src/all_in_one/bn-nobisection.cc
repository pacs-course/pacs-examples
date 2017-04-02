/*!
 * \file bn-allinone-nobisez.cpp
 * \brief all the exercise code in a single file, without bisection.
 * \date Oct 29, 2008
 */

#include <iostream>
#include <cmath>
#include <assert.h>
#include <cstdlib>

//! Type for real numbers
typedef double real;
//! Type for function pointers
typedef real (*fctptr)(real const x);
//! Type for convergence check
enum checkT {
  INCREMENT, //! Check the difference between subsequent iterates
  RESIDUAL,  //! Check the residual
  BOTH       //! Check both conditions
};

bool converged( real const & increment, real const & residual,
        real const & tol, checkT const & check ){
  /*
    Compares a parameter value against desired tolerance.
    The parameter is chosen upon the value of check.
  */
  switch(check){
  case INCREMENT:
      return (increment < tol);
  case RESIDUAL:
      return (residual < tol);
  case BOTH:
      return ((increment < tol)&&(residual < tol));
  }
}

/*
 *  Add bisection function here
 */

real newton(real xp, fctptr const f, fctptr const df,
        real const & tol, int const & maxit,
        checkT const & check, int & nit)
{
  real v = f(xp);
  real xnew;

  nit = 0;
  for(int k = 1; k <= maxit; ++k,++nit) {
    double derv = df(xp);
    if(!derv) {
      std::cerr << "ERROR: Division by 0 occurred in Newton algorithm"
        << std::endl;
      exit(1);
    }

    xnew = xp - v / derv;
    v = f(xnew);
    if(converged(fabs(xnew - xp), fabs(v),tol,check)) break;
    xp = xnew;
  }
  return xnew;
}


real robust(real a, real b,
        fctptr const f, fctptr const df,
        real const & tol, real const & cfratio,
        int const & maxit, checkT const & check,
        int & nit_coarse, int & nit_fine )
{
  // Call bisection method (with a greater desired tolerance)
  real tol_bis = cfratio * tol;
  /*
   * Add a call to bisection()
   */

  /*
    Call a Newton algorithm which uses as an initial value
    the solution given by bisection method
  */
  real x_bis=0;
  return newton(x_bis, f, df, tol, maxit, check, nit_fine);
}

//! Function f(x) from exercise 3
real f(real x) {
  return pow(x,2) - 0.5;
}

//! First derivative of function f(x) from exercise 3
real df(real x) {
  return 2. * x;
}

using namespace std;

int main() {

  int nit_bis;
  int nit_newt;

  // A convergent Newton call
  cout << newton(.1, f, df, 1e-8, 100,
         INCREMENT, nit_newt) << flush;
  cout << '\t' << nit_newt << endl;
  // A non-convergent Newton call
  cout << newton(.0, f, df, 1e-8, 100,
         INCREMENT, nit_newt) << flush;
  cout << '\t' << nit_newt << endl;

  /* A call to the robust method: it needs bisection()!
   *
  cout << robust(0., 1., f, df, 1e-8, 1e4, 100,
         INCREMENT, nit_bis, nit_newt)
       << flush;
  cout << '\t' << nit_bis
       << " "  << nit_newt << endl;
  */

  return 0;
}
