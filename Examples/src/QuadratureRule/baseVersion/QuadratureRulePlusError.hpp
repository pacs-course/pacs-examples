#ifndef QUADRATURERULEPLUSERROR_HPP
#define QUADRATURERULEPLUSERROR_HPP
#include <cmath>
#include "QuadratureRule.hpp"

namespace NumericalIntegration{

  //! A class that will record the error.
  /*
    Since the Quadrature rule is composed inside the class that
    implements composite integration, I need an external object to
    gather the information about the value of the error estimate of
    the given rule. I have chosen to use static members, but other
    solutions ar possible, for instance storing inside the
    QuadratureRulePlusError class a reference to an object whose role
    is to record the error. This latter solution is also safer, I have
    preferred the simpler one in this example.
   */
  struct ExtractError
  {
    static double error;
    static void reset(){error=0.0;}
  };

  double ExtractError::error(0);
  /*!
    \brief Standard quadrature rule plus error.

    This class implements the Decorator design pattern:
    it is implemented in terms of an object of the
    StandardQuadratureRule hierarchy and at the same time
    it derives from the base StandardQuadratureRule class.

    It purpose is to enrich the apply() method to calculate
    an estimate of the error using the classical formula based
    on comparing the integral over h and that over h/2.

    It uses the ExtractError class to pass out the information
    about the error.
   */

  template<typename SQR>
  class QuadratureRulePlusError: public QuadratureRule
  {
  public:
    QuadratureRulePlusError();
    //! Copy constructor.
    QuadratureRulePlusError(QuadratureRulePlusError const &)=default;
    //! Cloning method.
    QuadratureRuleHandler   clone() const;
    //! Number of quadrature nodes. Delagates to the stored rules.
    int num_nodes()const {return _therule.num_nodes();}
    //! Coordinate of i-th node. Delegates to the stored rules.
    double node(const int i) const {return _therule.node(i);};
    //! Weight of i-th node. Delegates to the stored rules.
    double weight(const int i)const{return _therule.weight(i);};
    //! Order of convergence of the rule.
    unsigned int order() const{return _therule.order();};
    //! The factor used to compute the errore estimate
    double factor()const {return _factor;}
    //! The actual rule.
    double apply(FunPoint const &, double const & a,
			 double const & b) const;
  protected:
    //! The underlying rule. static becouse it is common to all objects of the class
    static SQR _therule;
    //! Factor used in the calculation of the error estimate.
    double _factor;
  };
  template<typename SQR>
  SQR QuadratureRulePlusError<SQR>::_therule;
 

  template<typename SQR>
  QuadratureRulePlusError<SQR>::QuadratureRulePlusError():_factor(1./(1-std::pow(2,1.0-_therule.order())))
  {}

  template<typename SQR>
  QuadratureRuleHandler QuadratureRulePlusError<SQR>::clone() const
  { return  QuadratureRuleHandler(new QuadratureRulePlusError<SQR>(*this));}

  /*! 
    In the apply method I am using the error formula in a stange way
    because want to replicate exactly the behavior of the
    QuadratureBase classes. Thus I compute the error of the formula
    with step h even if I have available the result with step
    h/2. Consistently, I return the value for h and not h/2.
  */
  template<typename SQR>
  double QuadratureRulePlusError<SQR>::apply(FunPoint const & f, double const & a,
					double const & b) const
  {
    double result=_therule.apply(f,a,b);
    double xm=(a+b)/2.0;
    double result2=
      _therule.apply(f,a,xm)+_therule.apply(f,xm,b);
    // Record the error in the extractor.
    ExtractError::error+=(result2-result)*_factor;
    return result;// I should return result2 see commment above
  }
}




#endif
