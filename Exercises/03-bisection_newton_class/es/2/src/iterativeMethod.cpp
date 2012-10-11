#include "iterativeMethod.hpp"

namespace RootFinding {
  IterativeMethod::IterativeMethod(real tol, int maxit, checkT check)
    : M_tol(tol), M_maxit(maxit), M_check(check) {}

  bool IterativeMethod::converged(real increment, real residual) {
    return (M_check == INCREMENT && increment < M_tol) ||
      (M_check == RESIDUAL && residual < M_tol);
  }
}
