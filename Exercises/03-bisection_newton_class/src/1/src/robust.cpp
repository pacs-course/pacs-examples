#include "robust.hpp"

namespace RootFinding
{
  Robust::Robust(const real& tol, const int& maxit,
                 const checkT& check, const real& cfratio) :
    M_tol(tol),
    M_cfratio( cfratio ),
    M_maxit(maxit),
    M_check(check),
    M_coarse(M_cfratio * tol, maxit, check),
    M_fine(tol, maxit) {}

  real Robust::apply(real a, real b, fctptr f, fctptr df)
  {
    return M_fine.apply( M_coarse.apply(a, b, f), f, df );
  }
}
