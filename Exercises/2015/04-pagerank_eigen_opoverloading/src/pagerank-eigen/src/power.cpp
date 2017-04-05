#include "power.hpp"

namespace LinearAlgebra
{
  namespace Eigenvalues
  {

    double PowerMethod::apply(const Matrix& A, Vector& z0)
    {
      // Consistency check
      const unsigned int N = A.rows();
      assert(A.cols() == N && z0.size() == N);

      Vector q( z0 / z0.norm() );
      Vector z( N );
      Vector w( z0 / z0.norm() );
      Vector h( N );

      double res = M_tol + 1.;
      double costheta = 0.;

      M_nit = 0;
      while(res > M_tol && M_nit < M_maxnit)
      {
        M_nit++;
        // Approximation of the right eigenvector
        z = A * q;
        q = z / z.norm();
        // Approximation of the eigenvalue of largest module
        M_nu = q.transpose() * A * q;
        // Approximation of the left eigenvector
        h = w.transpose() * A;
        w = h / h.norm();

        // Angle between the left and right eigenvector
        costheta = fabs( w.transpose() * q );

        // Residual
        res = ( (z - M_nu * q ).norm() ) / costheta;
      }
      z0=q;
      return M_nu;
    }

    std::ostream& operator<<(std::ostream& ostr, const PowerMethod& pm)
    {
      ostr << "* POWER METHOD *" << std::endl
           << "# of iterations     : " << pm.nit() << std::endl
           << "Convergence         : "
           << ( ( pm.nit() == pm.maxnit() ) ? "NO" : "YES" )
           << std::endl
           << "Tolerance           : " << pm.tol() << std::endl
           << "Computed eigenvalue : " << pm.nu() << std::flush;

      return ostr;
    }

  } // namespace LinearAlgebra

} // namespace EigenValues
