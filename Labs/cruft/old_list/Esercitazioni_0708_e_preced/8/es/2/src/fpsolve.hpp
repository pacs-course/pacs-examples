/*!
  \file fpsolve.hpp
  \brief Iterative fixed point algorithm for root finding
  \author Tiziano Passerini
  \date 12/15/2006
*/

#ifndef __FPSOLVE_HPP
#define __FPSOLVE_HPP 1

#include <iostream>

namespace FixedPoint {

  /*!
    \struct ResType
    \brief Type for result
  */
  struct ResType {
    double val;    /*!< \f$f(x)\f$'s root */
    int nit;       /*!< number of iterations performed */
    bool status;   /*!< convergence check */
  };

  /*!
    \class SolveFP
    \brief Iterative algorithm
  */
  template< typename FP >
  class SolveFP
  {
  public:
    /** @name Constructors
     */
    //@{
    //! Constructor taking the function
    /*!
      \param tol tolerance
      \param maxit maximum number of iterations
      \param method fixed point method
    */
    SolveFP( const double& tol, const int& maxit, const FP& method ) :
      _tol( tol ),
      _maxit( maxit ),
      _method( method )
    {};
    //@}
    /** @name Methods
     */
    //@{
    //! Applies the method
    /*!
      \param x_0 initial guess of the solution
      \return an estimate of the zero of \f$f\f$
    */
    ResType iterate( const double& x_0 );
    //@}
  private:
    double _tol;
    int _maxit;
    FP _method;
  };


  template< typename FP >
  ResType
  SolveFP<FP>::iterate( const double& x_0 )
  {
    double x_k( x_0 );
    double increment;
    ResType r;
    r.nit = 0;
    
    do{
      r.val = _method.apply( x_k );
      increment = std::fabs(r.val - x_k);
      x_k = r.val;
      ++r.nit;
    }
    while( ( increment > _tol ) && ( r.nit < _maxit ) );
    
    ( r.nit < _maxit ) ? (r.status = true) : (r.status = false);
    
    return r;
  }
  
}
#endif
