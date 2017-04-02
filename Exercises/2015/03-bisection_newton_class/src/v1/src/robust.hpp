#ifndef ROBUST_HPP
#define ROBUST_HPP 1

#include "rootfinding.hpp"
#include "bisection.hpp"
#include "newton.hpp"

namespace RootFinding
{

  /*!
    \class Robust
    \brief Find the root of a non-linear equation
    using a robust method 
  */
  class Robust
  {
  public:
    //! Coarse method type
    typedef Bisection coarseT;
    //! Fine method type
    typedef Newton fineT;
    /** @name Constructors
     */
    //@{
    //! Constructor
    Robust(const real& tol, const int& maxit,
           const checkT& check = INCREMENT,
           const real& cfratio = 1.E4);
    //@}
    /** @name Accessors
     */
    //@{
    //! Return the number of iterations of the coarse method
    int nitcoarse() { return M_coarse.nit(); }
    //! Return the number to iterations of the fine method
    int nitfine() { return M_fine.nit(); }
    //@}
    /** @name Methods
     */
    //@{
    //! Apply the method
    real apply(real, real, fctptr, fctptr);
    //@}

  private:

    //! The tolerance
    const real M_tol;
    //! The ratio between coarse and fine method tolerance
    const real M_cfratio;
    //! The maximum number of iterations
    const int M_maxit;
    //! The check type
    const checkT M_check;
    //! The coarse method
    coarseT M_coarse;
    //! The fine method
    fineT M_fine;
  };
} // namespace RootFinding

#endif
