/*!
  \file ciclycJacobi.hpp
  \author Daniele A. Di Pietro <dipietro@unibg.it>
  \date 8-19-2005
*/

#ifndef CYCLICJACOBI_HPP
#define CYCLICJACOBI_HPP 1

#include "givens.hpp"

namespace LinearAlgebra {
  namespace Eigenvalues {
    /*!
      \class CyclicJacobi
      \brief Eigenvalue computation via ciclyc Jacobi method
      \author Daniele A. Di Pietro <dipietro@unibg.it>
    */

    class CyclicJacobi {
    public:
      /** @name Constructors
       */
      //@{
      //! Constructor taking the tolerance
      CyclicJacobi(double tol) : M_tol(tol) {}
      //@}
      /** @name Accessors
       */
      //@{
      //! Return the tolerance
      inline double& tol() { return M_tol; }
      //! Return the tolerance (const version)
      inline const double& tol() const { return M_tol; }
      //@}
      /** @name Members
       */
      //@{
      //! Apply cyclic Jacobi method
      Vector apply(const Matrix&);
      //@}

      /** @name Output
       */
      //@{
      //! Send results to an output stream
      friend std::ostream& operator<<(std::ostream&, const CyclicJacobi&);
      //@}
    private:
      //! Compute c and s
      void symSchur(const Matrix&, int, int, double&, double&);
      //! Number of sweeps
      int M_sweep;
      //! Tolerance
      double M_tol;
      //! Psi norm of matrix D
      double M_psi;
      //! Stopping check parameter
      double M_eps;
    };
  } // namespace Eigenvalues
} // namespace LinearAlgebra
#endif
