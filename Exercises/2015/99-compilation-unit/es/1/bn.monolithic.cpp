/*!
  \file bn.monolithic.cpp
  \brief Compilable file for exercise 4

  This file contains all the functions
  and the types required in exercise 4

  \author Daniele A. Di Pietro
  \author Luca Formaggia
  \author Tiziano Passerini
  \date 10-25-2006
*/

#include <iostream>
#include <algorithm>
#include <cmath>

//! Type for real numbers
typedef double real;
//! Type for function pointers
typedef real (*fctptr)(real const x);
//! Type for convergence check
enum checkT {
  INCREMENT, //!< Check the difference between subsequent iterates
  RESIDUAL   //!< Check the residual
};

/*!
  \brief Convergence check

  Given the stopping criterion for an iterative algorithm (which is
  specified in \e check parameter) this function compares the
  increment or the residual computed in the last iteration against
  the desired tolerance.

  \param increment norm of the difference between two subsequent
  iterates
  \param residual \f$f(\alpha)\f$ being \f$\alpha\f$ the approximation
  of the zero of \f$f\f$
  \param tol tolerance for the approximate solution
  \param check type of convergence check (INCREMENT or RESIDUAL)
*/
bool converged( real const & increment, real const & residual,
		real const & tol, checkT const & check ){
  /*
    Compares a parameter value against desired tolerance.
    The parameter is chosen upon the value of check.
  */
  return (check == INCREMENT && increment < tol) ||
    (check == RESIDUAL && residual < tol);
}


//! Bisection method
/*!
  \param a interval boundary
  \param b interval boundary
  \param f pointer to function f(x)
  \param maxit maximum number of iterations
  \param nit actual number of iterations performed by the algorithm

  Note that \e a and \e b are not defined constant: the algorithm
  needs to update the interval boundaries
*/
real bisection(real a, real b, fctptr const f, 
	       real const & tol, int const & maxit,
	       checkT const & check, int & nit)
{
  real u = f(a);  // Evaluate f on boundary a
  real l = b - a; // Interval length
  real r;         // Residual
  real c;         // Middle point

  nit = 0;
  for(int k = 1; k <= maxit; ++k,++nit) {
    l *= 0.5;
    c = a + l;
    real r = f(c);                

    if( converged(fabs(l), fabs(r), tol, check) ) break;

    ((u > 0. && r < 0.) || (u < 0. && r > 0.)) ? (b = c) :
      (a = c, u = r);
  }

    return c;
}


//! Newton method
/*!
  \param xp initial value
  \param f pointer to function f(x)
  \param df pointer to function derivative [df/dx](x)

  Note that xp is not defined constant: it will contain the
  approximate solution after each iteration
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


//! Robust method
/*!
  This algorithm combines a low order method with a higher
  order one.
  Starting from an estimate of the solution, obtained from bisection
  method, the Newton method recovers an approximate solution within
  the desired tolerance \e tol.

  \param cfratio ratio between tolerance values required to the two
  different algorithms:
  \f[
  tol_{bisection} = cfratio * tol_{newton}
  \f]
*/
real robust(real a, real b,
	    fctptr const f, fctptr const df,
	    real const & tol, real const & cfratio, 
	    int const & maxit, checkT const & check,
	    int & nit_coarse, int & nit_fine )
{
  // Call bisection method (with a greater desired tolerance)
  real tol_bis = cfratio * tol;
  real x_bis = bisection(a, b, f, tol, maxit, check, nit_coarse);

  /*
    Call a Newton algorithm which uses as an initial value
    the solution given by bisection method
  */
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


//! Main function
/*!
  \brief Solve exercise 3
*/
int main() {

  using namespace std;

  int nit_bis;
  int nit_newt;

  cout << bisection(0., 1., f, 1e-8, 100, INCREMENT, nit_bis)
       << flush;
  cout << '\t' << nit_bis << endl;
  cout << newton(.1, f, df, 1e-8, 100, INCREMENT, nit_newt)
       << flush;
  cout << '\t' << nit_newt << endl;
  cout << robust(0., 1., f, df, 1e-8, 1e4, 100, INCREMENT,
		 nit_bis, nit_newt)
       << flush;
  cout << '\t' << nit_bis
       << " "  << nit_newt << endl;
  cout << newton(0., f, df, 1e-8, 100, INCREMENT, nit_newt)
       << flush;
  cout << '\t' << nit_newt << endl;

  return 0;
}
