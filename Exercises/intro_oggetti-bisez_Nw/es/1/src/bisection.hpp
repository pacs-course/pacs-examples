#ifndef BISECTION_HPP
#define BISECTION_HPP 1

#include "rootfinding.hpp"
#include <assert.h>

namespace RootFinding
{
  /*!
    \class Bisection
    \brief Find the root of a non-linear equation using
    the bisection method

    This class provides an implementation of the Bisection
    method for rootfinding.
  */
  class Bisection 
  {
  public:
    /** @name Constructors
     */
    //@{
    //! Constructor   
    Bisection(const real& tol, const int& maxit,
	      const checkT& check);
    //@}
    /** @name Accessors
     */
    //@{
    //! Return the tolerance
    real tol() const { return M_tol; }
    //! Return the maximum number of iterations
    int maxit() const { return M_maxit; }
    //! Return the number of iterations at last call
    int nit() const { return M_nit; }
    //@}
    /** @name Methods
     */
    //@{
    //! Apply the method
    real apply(real, real, fctptr);
    //@}
  private:
    //! Check if the method has converged
    bool inline converged(const real&, const real&) const;
    //! Tolerance
    const real M_tol;
    //! Maximum number of iterations
    const int M_maxit;
    //! Number of iteration at last call
    int M_nit;
    //! Type for the convergence check
    const checkT M_check;
  };

bool inline Bisection::converged(const real& increment,
				   const real& residual) const
  {
      switch(M_check){
      case INCREMENT:
          return (increment < M_tol);
      case RESIDUAL:
          return (residual < M_tol);
      case BOTH:
          return ((increment < M_tol)&&(residual < M_tol));
      default:
          return false;
      }
  }

} // namespace RootFinding

#endif
