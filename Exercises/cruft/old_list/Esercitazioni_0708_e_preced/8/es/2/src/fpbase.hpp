/*!
  \file fpbase.hpp
  \brief Iteration functions for fixed point iterative methods
  \author Tiziano Passerini
  \date 12/15/2006
*/

#ifndef __FPBASE_HPP
#define __FPBASE_HPP 1

namespace FixedPoint {

  //! Type for function pointers
  typedef double (*Funp)(double const &);

  /*!
    \class FixedPointBase
    \brief An abstract class describing an iteration function
  */
  class FixedPointBase
  {
  public:
    /** @name Methods
     */
    //@{
    //! Applies the iterating function: x_(k+1) = phi ( x_k )
    /*!
      \param x_k solution at current iteration
      \return solution at next iteration
    */
    virtual double apply( const double& x_k ) = 0;
    //@}

    /* if you want to define this class as a functor use the following */
    //    double operator()( const double& x_k );
  };

}

#endif
