#ifndef QUADRATURERULEPLUSERROR_HPP
#define QUADRATURERULEPLUSERROR_HPP
#include <cmath>

#include "QuadratureRuleBase.hpp"

namespace apsc::NumericalIntegration{


  /*!
    \brief Standard quadrature rule plus computation of error estimate.

    This class implements the Decorator design pattern:
    it is implemented in terms of an object of the
    StandardQuadratureRule hierarchy and at the same time
    it derives from the base StandardQuadratureRule class.

    It purpose is to enrich the apply() method to calculate
    an estimate of the error using the classical formula based
    on comparing the integral over h and that over h/2.

    Since the Quadrature rule is composed inside the class that
    implements composite integration, I need a way to
    gather the information about the value of the error estimate of
    the given rule. I have chosen to use an mutable variable of an embedded struct
   */

  template<typename SQR>
  class QuadratureRulePlusError final: public QuadratureRuleBase
  {
  public:
    QuadratureRulePlusError();
    //! Cloning method.
    std::unique_ptr<QuadratureRuleBase>   clone() const override;
    //! Number of quadrature nodes. Delagates to the stored rules.
    int num_nodes()const {return _therule.num_nodes();}
    //! Coordinate of i-th node. Delegates to the stored rules.
    double node(const int i) const {return _therule.node(i);};
    //! Weight of i-th node. Delegates to the stored rules.
    double weight(const int i)const{return _therule.weight(i);};
    //! Order of convergence of the rule.
    unsigned int order() const{return _therule.order();};
    //! The factor used to compute the error estimate
    double factor()const {return _factor;}
    //! The actual rule.
    double apply(FunPoint const &, double const & a,
			 double const & b) const override;
    //! String to identify the rule type
    std::string name()const override{return "QuadratureRulePlusError";}
    //! Internal class to store the error
    struct Error
     {
       double error=0.0;
       void reset(){error=0.0;}
     };
    //! The error.
    //! @note mutable since it may change even on const object. Indeed apply() is const
    //! since it override a const method of the base class.
    mutable Error ExtractError;
    protected:

    //! The underlying rule. static because it is common to all objects of the class
    //! inlined so this is a definition (since C++17)
    inline static SQR _therule;
    //! Factor used in the calculation of the error estimate.
    double _factor;

  };
  // Not needed in C++17
  //template<typename SQR>
  //SQR QuadratureRulePlusError<SQR>::_therule;
 

  template<typename SQR>
  QuadratureRulePlusError<SQR>::QuadratureRulePlusError():_factor(1./(1-std::pow(2,1.0-_therule.order())))
  {}

  template<typename SQR>
  std::unique_ptr<QuadratureRuleBase> QuadratureRulePlusError<SQR>::clone() const
  { return  std::unique_ptr<QuadratureRuleBase>(new QuadratureRulePlusError<SQR>(*this));}

  /*! 
    In the apply method I am using the error formula in a strange way
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
    ExtractError.error+=(result2-result)*_factor;
    return result;// I should return result2 see commment above
  }
}




#endif
