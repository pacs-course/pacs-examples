#ifndef NEWTON_HPP
#define NEWTON_HPP 1

#include "iterativeMethod.hpp"

namespace RootFinding {
  /*!
    \class Newton
    \brief Find the root of a non-linear equation using the Newton method    
    \author Daniele A. Di Pietro
    \author Paolo G. Ferrandi
    \date 8-26-2005
  */
  class Newton : public IterativeMethod {
  public:
    /** @name Constructors
     */
    //@{
    //! Constructor taking the tolerance and the maximum number of 
    //! iterations
    Newton(real, int);
    //@}
    /** @name Methods
     */
    //@{
    //! Apply the method
    real apply(real, fctptr, fctptr);
    //@}
  };
}

#endif
