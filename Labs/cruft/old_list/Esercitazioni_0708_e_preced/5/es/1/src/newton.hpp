#ifndef NEWTON_HPP
#define NEWTON_HPP 1

#include "rootfinding.hpp"

namespace RootFinding {
  /*!
    \class Newton
    \brief Find the root of a non-linear equation using
    the Newton method
    \author Daniele A. Di Pietro
    \author Tiziano Passerini
    \author Paolo G. Ferrrandi
    \date 11-06-2007
  */
  class Newton {
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
    inline const real tol() const { return M_tol; }
    //! Return the maximum number of iterations
    inline const int maxit() const { return M_maxit; }
    //! Return the number of iterations at last call
    inline const int nit() const { return M_nit; }
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
}

#endif
