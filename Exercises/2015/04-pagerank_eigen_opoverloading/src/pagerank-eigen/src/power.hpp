/*!
  \file power.hpp
*/

#ifndef POWER_HPP
#define POWER_HPP 1

#include <iostream>
#include <Eigen/Sparse>

namespace LinearAlgebra
{
  namespace Eigenvalues
  {
    /*!
      \class PowerMethod
      \brief Implementation of the power method to compute
      the eigenvalue of largest module
    */

    class PowerMethod
    {
    public:
      /* @name Typedef
        Typedef for matrices and vectors
       */
      typedef Eigen::MatrixXd Matrix;
      typedef Eigen::VectorXd Vector;
      /** @name Constructors and Destructors
       */
      //@{
      //! Constructor taking the tolerance and the maximum number of iterations
      PowerMethod(double tol=1.e-6, unsigned int maxnit=100) :
        M_tol(tol), M_maxnit(maxnit), M_nu(0.) {}
      //! Destructor
      virtual ~PowerMethod() {};
      //@}

      PowerMethod ( const PowerMethod& ) = delete;

      PowerMethod& operator = ( const PowerMethod& ) = delete;

      /** @name Accessors
       */
      //@{
      //! Return the tolerance
      double& tol() { return M_tol; }
      //! Return the tolerance (const version)
      const double& tol() const { return M_tol; }
      //! Return the number of iterations needed by the last call of
      //! apply()
      unsigned int nit() const { return M_nit; }
      //! Return the maximum number of iterations
      unsigned int& maxnit() { return M_maxnit; }
      //! Return the maximum number of iterations (const version)
      unsigned const int& maxnit() const { return M_maxnit; }
      //! Return the estimated eigenvalue
      double nu() const { return M_nu; }
      //@}

      /** @name Members
       */
      //@{
      double apply( const Matrix& A, Vector& z0);
      //@}

      /** @name Output
       */
      //@{
      friend std::ostream& operator<<(std::ostream& ostr,
                                      const PowerMethod& pm);
      //@}
    private:

      /** @name Private copy constructor
       */
      //@{
      // PowerMethod ( const PowerMethod& );
      //@}
      /** @name Private assign operator
       */
      //@{
      // PowerMethod& operator = ( const PowerMethod& );
      //@}

      //! Tolerance
      double M_tol;
      //! Number of iterations
      unsigned int M_nit;
      //! Maximum number of iterations
      unsigned int M_maxnit;
      //! Computed eigenvalue
      double M_nu;
    };

  } // namespace LinearAlgebra

} // namespace Eigenvalues
#endif
