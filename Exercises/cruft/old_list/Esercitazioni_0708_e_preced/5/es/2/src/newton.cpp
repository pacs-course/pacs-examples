#include "newton.hpp"

namespace RootFinding {
  Newton::Newton(real tol, int maxit) 
    : IterativeMethod(tol, maxit, INCREMENT) {}
        
  real Newton::apply(real xp, fctptr f, fctptr df){
    real v = f(xp);
    real xv;
    real derv;
    
	M_nit = 0;
	
	do {
		xv = xp;
		derv = df(xv);
      	if(!derv) {
		std::cerr << "ERROR: Division by 0 occurred "
				"in Newton::apply()" << std::endl;
			exit(1);
      	}
      	xp = xv - v / derv;
      	v = f(xp);
      	++M_nit;
    } while ( !(converged(fabs(xp - xv), fabs(v))) && (M_nit <= M_maxit) );
  return xp;
  }
}
