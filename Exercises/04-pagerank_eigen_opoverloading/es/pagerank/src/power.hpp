/*!
  \file power.hpp
  \author Daniele A. Di Pietro
  \date 19-8-2005
*/

#ifndef POWER_HPP
#define POWER_HPP 1

#include "linearAlgebra.hpp"

namespace LinearAlgebra {
  namespace Eigenvalues {
    /*!
      \class PowerMethod
      \brief Implementation of the power method to compute
      the eigenvalue of largest module
      \author Daniele A. Di Pietro
    */

    class PowerMethod {
    public:
      /** @name Constructors
       */
      //@{
      //! Constructor taking the tolerance and the maximum number of iterations
      PowerMethod(double tol, int maxnit) :
	M_tol(tol), M_maxnit(maxnit), M_nu(0.) {}
      //@}

      /** @name Accessors
       */
      //@{
      //! Return the tolerance
      inline double& tol() { return M_tol; }
      //! Return the tolerance (const version)
      inline const double& tol() const { return M_tol; }
      //! Return the number of iterations needed by the last call of
      //! apply()
      inline int nit() const { return M_nit; }
      //! Return the maximum number of iterations
      inline int& maxnit() { return M_maxnit; }
      //! Return the maximum number of iterations (const version)
      inline const int& maxnit() const { return M_maxnit; }
      //! Return the estimated eigenvalue
      inline double nu() const { return M_nu; }
      //@}

      /** @name Members
       */
      //@{
      double apply(const Matrix& A, Vector& z0);
      //@}

      /** @name Output
       */
      //@{
      friend std::ostream& operator<<(std::ostream& ostr,
				      const PowerMethod& pm);
      //@}
    private:
      //! Tolerance
      double M_tol;
      //! Number of iterations
      int M_nit;
      //! Maximum number of iterations
      int M_maxnit;
      //! Computed eigenvalue
      double M_nu;
    };

  } // namespace LinearAlgebra
} // namespace Eigenvalues
#endif
