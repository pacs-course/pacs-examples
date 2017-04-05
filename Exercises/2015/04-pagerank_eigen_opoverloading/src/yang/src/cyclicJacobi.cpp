#include "cyclicJacobi.hpp"

namespace LinearAlgebra {
  namespace Eigenvalues {
    void CyclicJacobi::symSchur(const Matrix& A, int p, int q, 
				double& c, double& s) {
      if(A(p, q) == 0.) { 
	c = 1.; s = 0.;   
      } else {
	double eta = ( A(q, q) - A(p, p) ) / ( 2 * A(p, q) );
	double t = (eta >= 0) ? 1 / (eta + sqrt(1 + pow(eta, 2))) :
	  -1 / (-eta + sqrt(1 + pow(eta, 2)));

	c = 1 / sqrt(1 + pow(t,2));
	s = c * t;
      }
    }

    Vector CyclicJacobi::apply(const Matrix& A) {
      // Check that A is square
      assert(A.size1() == A.size2());

      Matrix D(A);
      M_eps = M_tol * norm_fro(A);
      M_psi = norm_psi(D);
      M_sweep = 0;

      double c, s;

      while(M_psi > M_eps) {
	M_sweep++;
	for(int p = 0; p < A.size1() - 1; p++)
	  for(int q = p + 1; q < A.size2(); q++) {
	    symSchur(D, p, q, c, s);
	    Givens gt(p, q, c, s);
	    gt.apply( D );
	  }
	M_psi = norm_psi(D);
      }

      Vector v = diag(D);

      return v;
    }
  } // namespace LinearAlgebra
} // namespace EigenValues
