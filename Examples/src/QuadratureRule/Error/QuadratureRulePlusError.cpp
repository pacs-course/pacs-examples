#include "QuadratureRulePlusError.hpp"
namespace NumericalIntegration{

  double ExtractError::error(0);

  void  ExtractError::reset(){error=0;};


  // Here a cannot use simply clone() since it returns a unique_ptr to a 
  // QuadratureRule and not StandardQuadratureRule, so I need to get the
  // raw pointer using release()
  QuadratureRulePlusError::QuadratureRulePlusError(StandardQuadratureRule const & e):
    _factor(1./(1-std::pow(2,1.0-e.order()))),
    _therule(static_cast<StandardQuadratureRule *>(e.clone().release()))
  {}

  QuadratureRulePlusError::QuadratureRulePlusError(StandardQuadratureRule && e):
    _factor(1./(1-std::pow(2,1.0-e.order()))),
    _therule(static_cast<StandardQuadratureRule *>(e.clone().release()))
  {}


  QuadratureRulePlusError::QuadratureRulePlusError(QuadratureRulePlusError const & rhs):
    _factor(rhs._factor),_therule(static_cast<StandardQuadratureRule *>(rhs._therule->clone().release())){}


  QuadratureRuleHandler QuadratureRulePlusError::clone() const
  { return  QuadratureRuleHandler(new QuadratureRulePlusError(*this));}

  /*! 
    In the apply method I am using the error formula in a stange way
    because want to replicate exactly the behavior of the
    QuadratureBase classes. Thus I compute the error of the formula
    with step h even if I have available the result with step
    h/2. Consistently, I return the value for h and not h/2.
  */
  double QuadratureRulePlusError::apply(FunPoint const & f, double const & a,
					double const & b) const
  {
    double result=_therule->apply(f,a,b);
    double xm=(a+b)/2.0;
    double result2=
      _therule->apply(f,a,xm)+_therule->apply(f,xm,b);
    // Record the error in the extractor.
    ExtractError::error+=(result2-result)*_factor;
    return result;// I should return result2 see commment above
  }
}



