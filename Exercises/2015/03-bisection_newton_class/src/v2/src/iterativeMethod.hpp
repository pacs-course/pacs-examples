#ifndef ITERATIVEMETHOD_HPP
#define ITERATIVEMETHOD_HPP 1

#include "rootfinding.hpp"

namespace RootFinding {
  /*!
    \class IterativeMethod
    \brief Base class for iterative methods
    \author Daniele A. Di Pietro
    \date 8-26-2005
  */
  class IterativeMethod {
  public:
    //! Type for convergence check
    enum checkT {
      INCREMENT, //!< Check the difference between subsequent iterates
      RESIDUAL   //!< Check the residual
    };
    /** @name Constructors
     */
    //@{
    IterativeMethod(real tol, int maxit, checkT check);
    //@}
    /** @name Accessors
     */
    //@{
    //! Return the tolerance
    inline real& tol() { return M_tol; }
    //! Return the maximum number of iterations
    inline int& maxit() { return M_maxit; }
    //! Return the number of iterations at last call
    inline int nit() const { return M_nit; }
    //@}
  protected:
    //! Check if the method has converged
    bool converged(real, real);
    //! Number of iterations at last call
    int M_nit;
    //! Tolerance
    real M_tol;
    //! Maximum number of iterations
    int M_maxit;
    checkT M_check;
  };
}

#endif
