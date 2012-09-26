#include "bisection.hpp"

namespace RootFinding 
{
  Bisection::Bisection(const real& tol, const int& maxit,
		       const checkT& check)
    : M_tol(tol), M_maxit(maxit), M_check(check) {}

  real Bisection::apply(real a, real b, fctptr f){
    real u = f(a);	// Evaluate f on boundary a
    real l = b - a; // Interval length
    real r;         // Residual
    real c = a + l; // Middle point

    M_nit = 0;
    r = f(c);

    assert(u*f(b)<0.0);

    while ( !(converged(fabs(l), fabs(r)))
    		&& (M_nit <= M_maxit) ) {

		(u*r < 0.) ? (b = c) : (a = c, u = r);
		l *= 0.5;
		c = a + l;
		r = f(c);
		++M_nit;
	}
     return c;
  }

} // namespace RootFinding
