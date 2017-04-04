/*!
  \file numerical_integration.hpp
  \brief Implement a numerical integrator 
  \author Luca Formaggia
*/

#ifndef __NUMERICAL_INTEGRATION_HPP
#define __NUMERICAL_INTEGRATION_HPP 1

#include <vector>

#include "numerical_rule.hpp"

namespace NumericalIntegration{

  //! Type for function pointer (could use functors instead)
  typedef double (*FunPoint)(double const &);


  /*!
    \class Quadrature
    \brief A numerical integrator for f(x) in (a,b)
  */
  class Quadrature
  {
  public:
    /** @name Constructors
     */
    //@{
    //! Constructor
    /*!
      \param rule quadrature rule
      \param a left boundary
      \param b right boundary
      \param Nint number of points inside (a, b)
    */
    Quadrature(QuadratureRule & rule,
	       double const & a, double const & b, int const & Nint);
    //@}
    /** @name Methods
     */
    //@{
    //! Find the approximated integral of f(x)
    double apply(FunPoint f);
    //@}
  protected:
    QuadratureRule & _rule;
    double _a,_b,_h;
    double _wf;
    int _nint;
    std::vector<double> _knots;
  };
  
}

#endif
