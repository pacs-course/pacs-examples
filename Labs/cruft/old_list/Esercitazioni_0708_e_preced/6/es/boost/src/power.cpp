#include "power.hpp"

namespace LinearAlgebra {
  namespace Eigenvalues {
    
    double PowerMethod::apply(const Matrix& A, const Vector& z0) {
      // Consistency check
      int N = A.size1();
      assert((int)A.size2() == N && (int)z0.size() == N);

      Vector q( z0 / norm_2(z0) );
      Vector z( N );
      Vector w( N );
      
      double res = M_tol + 1.; 
      double costheta = 0.;

      M_nit = 0;
      while(res > M_tol && M_nit < M_maxnit) {
	M_nit++;

	// Approximation of the right eigenvector
	z = prod(A, q);
	q = z / norm_2(z);

	// Approximation of the eigenvalue of largest module
	M_nu = inner_prod(q, prod(q, A));

	// Approximation of the left eigenvector
	w = prod(q, A);
	w /= norm_2(w);

	// Angle between the left and right eigenvector
	costheta = fabs( inner_prod(w, q) );

	// Residual
	res = norm_2( (z - M_nu * q ) ) / costheta;
      }

      return M_nu;
    }

    std::ostream& operator<<(std::ostream& ostr, const PowerMethod& pm) {
      ostr << "* POWER METHOD *" << std::endl;
      ostr << "# of iterations     : " << pm.nit() << std::endl;
      ostr << "Convergence         : " 
	   << ( ( pm.nit() == pm.maxnit() ) ? "NO" : "YES" )
	   << std::endl;
      ostr << "Tolerance           : " << pm.tol() << std::endl;
      ostr << "Computed eigenvalue : " << pm.nu() << std::flush;
      return ostr;
    }
  } // namespace LinearAlgebra
} // namespace EigenValues
