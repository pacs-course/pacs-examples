/*!
  \file fpdummy.hpp
  \brief Iteration functions for fixed point iterative methods
  \author Tiziano Passerini
  \date 12/15/2006
*/

#ifndef __FPDUMMY_HPP
#define __FPDUMMY_HPP 1

#include "fpbase.hpp"

namespace FixedPoint {

  /*!
    \class Dummy
    \brief A very simple iteration function
  */
  class Dummy :
    public FixedPointBase
  {
  public:
    /** @name Constructors
     */
    //@{
    //! Constructor taking the function
    /*!
      \param f the function whose zero we are looking for
    */
    Dummy( const Funp& f );
    //@}
    /** @name Methods
     */
    //@{
    //! Applies the iteration function: x_(k+1) = phi ( x_k )
    /*!
      \param x_k solution at current iteration
      \return solution at next iteration
    */
    double apply( const double& x_k );
    //@}

    //    double operator()( const double& x_k );

  private:
    Funp _f;
  };

}

#endif
