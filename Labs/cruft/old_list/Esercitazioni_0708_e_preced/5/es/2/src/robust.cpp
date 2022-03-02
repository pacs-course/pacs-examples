#include "robust.hpp"

namespace RootFinding {
  Robust::Robust(real tol, int maxit, checkT check, real cfratio)
    :
    IterativeMethod(tol, maxit, check),
    M_cfratio(cfratio),
    M_coarse(M_cfratio * tol, maxit, check),
    M_fine(tol, maxit) {}

  real Robust::apply(real a, real b, fctptr f, fctptr df) {
    return M_fine.apply( M_coarse.apply(a, b, f), f, df );
  }

  std::ostream& operator<<(std::ostream& ostr, Robust& r) {
    ostr << "* Robust Method *" << std::endl;
    ostr << "Tolerance           :" << '\t' << r.M_tol 
	 << std::endl;
    ostr << "Max # of iterations :" << '\t' << r.M_maxit
	 << std::endl;
    ostr << "Convergence check   :" << '\t'
	 << ((r.M_check == IterativeMethod::INCREMENT) ? 
	     "increment" : "residual")
	 << std::endl;
    ostr << "# of iterations (C) :" << '\t' << r.coarse().nit() 
	 << std::endl;
    ostr << "# of iterations (F) :" << '\t' << r.fine().nit()
	 << std::endl;
    ostr << "C-to-F tol ratio    :" << '\t' << r.M_cfratio
	 << std::flush;
  }
}
