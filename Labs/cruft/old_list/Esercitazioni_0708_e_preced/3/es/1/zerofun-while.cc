/*!
  \file zerofun.cpp
  \brief Implementation of algorithms
  
  \author Daniele A. Di Pietro
  \author Luca Formaggia
  \author Tiziano Passerini
  \date 25-10-2006
*/

#include <assert.h>
#include "zerofun.hpp"


bool converged( real const & increment, real const & residual,
		real const & tol, checkT const & check ){
  /*
    Compares a parameter value against desired tolerance.
    The parameter is chosen upon the value of check.
  */
  return (check == INCREMENT && increment < tol) ||
    (check == RESIDUAL && residual < tol);
}


real bisection(real a, real b, fctptr const f, 
	       real const & tol, int const & maxit,
	       checkT const & check, int & nit)
{
	real u = f(a);  // Evaluate f on boundary a
	real l = b - a; // Interval length
	real r;         // Residual
	real c = a+l;   // Middle point
  
	nit = 0;       
	r = f(c); 
	
	assert(u*f(b)<0.0);
	
	while ( !(converged(fabs(l), fabs(r), tol, check)) 
			&& (nit <= maxit) ) {
				
		(u*r < 0.) ? (b = c) : (a = c, u = r);	
		l *= 0.5;
		c = a + l;
		r = f(c);
		++nit;
	}
	return c;
}

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
  real x_bis = bisection(a, b, f, tol, maxit, check, nit_coarse);

  /*
    Call a Newton algorithm which uses as an initial value
    the solution given by bisection method
  */
  return newton(x_bis, f, df, tol, maxit, check, nit_fine);
}



