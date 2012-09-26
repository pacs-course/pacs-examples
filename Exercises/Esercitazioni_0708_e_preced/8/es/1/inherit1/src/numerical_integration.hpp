/*!
  \file numerical_integration.hpp
  \brief A collection of classes for numerical integration
  \author Luca Formaggia
*/

#ifndef __NUMERICAL_INTEGRATION_HPP
#define __NUMERICAL_INTEGRATION_HPP 1

#include <vector>

namespace NumericalIntegration{


  //! Type for function pointers (could use functors instead)
  typedef double (*FunPoint)(double const &);
  

  /*!
    \class QuadratureBase
    \brief Approximate the integral of function f(x)
    over the interval (a, b)
  */
  class QuadratureBase
  {
  public:
    /** @name Constructors
     */
    //@{
    //! Constructor
    /*!
      \param a left boundary
      \param b right boundary
      \param Nint number of points inside (a, b)
    */
    QuadratureBase(double const & a, double const & b, int const & Nint);
    //@}
    /** @name Methods
     */
    //@{
    //! Find the approximated integral of f(x)
    virtual double apply(FunPoint f) const =0;
    //@}
  protected:
    double _a,_b,_h;
    int _nint;
    std::vector<double> _knots;  /* List of points inside interval (a, b) */
  };
  

  /*!
    \class Simpson
    \brief Integrator using Simpson formula
  */
  class Simpson : public QuadratureBase
  {
  public:
    /** @name Constructors 
     */
    //@{
    //! Constructor
    Simpson(double const & a, double const & b, int const & Nint);
    //@}
    /** @name Methods
     */
    //@{
    //! Apply the method
    virtual double apply(FunPoint f) const;
    //@}
  };


  /*!
    \class MidPoint
    \brief Integrator using mid point formula
  */
  class MidPoint : public QuadratureBase
  {
  public:
    /** @name Constructors 
     */
    //@{
    //! Constructor
    MidPoint(double const & a, double const & b, int const & Nint);
    //@}
    /** @name Methods
     */
    //@{
    //! Apply the method
    virtual double apply(FunPoint f) const;    
    //@}
  };

}

#endif
