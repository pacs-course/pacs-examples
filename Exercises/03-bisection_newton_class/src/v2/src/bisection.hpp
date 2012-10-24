#ifndef BISECTION_HPP
#define BISECTION_HPP 1

#include "iterativeMethod.hpp"

namespace RootFinding {
  /*!
    \class Bisection
    \brief Find the root of a non-linear equation using the bisection method

    This class provides an implementation of the Bisection method for
    rootfinding.
    \author Daniele A. Di Pietro
    \author Paolo G. Ferrandi
    \date 8-26-2005
  */
  class Bisection : public IterativeMethod {
  public:
    /** @name Constructors
     */
    //@{
    //! Constructor   
    Bisection(real tol, int maxit, checkT check);
    //@}

    /** @name Methods
     */
    //@{
    //! Apply the method
    real apply(real, real, fctptr);
    //@}
  };
}

#endif
