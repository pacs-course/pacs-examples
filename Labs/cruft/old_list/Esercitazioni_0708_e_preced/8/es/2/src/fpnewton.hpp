/*!
  \file fpnewton.hpp
  \brief Implementation for Newton fixed point method
  \author Tiziano Passerini
  \date 12/15/2006
*/

#ifndef __FPNEWTON_HPP
#define __FPNEWTON_HPP 1

#include "fpbase.hpp"

namespace FixedPoint {

  /*!
    \class Newton
    \brief Newton iteration function
  */
  class Newton :
    public FixedPointBase
  {
  public:
    /** @name Constructors
     */
    //@{
    //! Constructor taking the function and its derivative
    /*!
      \param f the function whose zero we are looking for
      \param df the derivative of \f$f\f$
    */
    Newton( const Funp& f, const Funp& df );
    //@}
    /** @name Methods
     */
    //@{
    //! Applies the iterating function: x_(k+1) = phi ( x_k )
    /*!
      \param x_k solution at current iteration
      \return solution at next iteration
    */
    double apply( const double& x_k );
    //@}

    //    double operator()( const double& x_k );

  private:
    Funp _f;
    Funp _df;
  };

}

#endif
