#ifndef NEWTON_HPP
#define NEWTON_HPP 1

#include "rootfinding.hpp"

namespace RootFinding
{
  /*!
    \class Newton
    \brief Find the root of a non-linear equation using
    the Newton method
  */
  class Newton 
  {
  public:
    /** @name Constructors
     */
    //@{
    //! Constructor taking the tolerance and the
    //! maximum number of iterations
    Newton(const real&, const int&);
    //@}
    /** @name Accessors
     */
    //@{
    //! Return the tolerance
    const real tol() const { return M_tol; }
    //! Return the maximum number of iterations
    const int maxit() const { return M_maxit; }
    //! Return the number of iterations at last call
    const int nit() const { return M_nit; }
    //@}
    /** @name Methods
     */
    //@{
    //! Apply the method
    real apply(real, fctptr, fctptr);
    //@}
  private:
    //! Check if the method has converged
    bool inline converged(const real&, const real&) const;
    //! Tolerance
    const real M_tol;
    //! Maximum number of iterations
    const int M_maxit;
    //! Number of iterations at last call
    int M_nit;
    //! Convergence check
    const checkT M_check;
  };

bool inline Newton::converged(const real& increment,
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
