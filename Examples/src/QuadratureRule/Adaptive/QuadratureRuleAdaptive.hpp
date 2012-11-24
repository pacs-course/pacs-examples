#ifndef QUADRATURERULEADAPTIVE_HPP
#define QUADRATURERULEADAPTIVE_HPP
#include <cmath>
#include <stdexcept>
#include "QuadratureRulePlusError.hpp"

namespace NumericalIntegration{

  /*!
    \brief Adaptive quadrature rules based on StandardQuadratureRule

    This class implements the Decorator design pattern.
    It is implemented in terms of a QuadratureRulePlusError.
   */

  class QuadratureRuleAdaptive: public StandardQuadratureRule
  {
  public:
    //! Constructor.
    /*!
      It takes compulsorily in input  a standard quad rule and information on the target error. Optionally
      you may set the total numer of iterations, which limits the number of subintervals used in the adaptive
      algorithm.
    */
    QuadratureRuleAdaptive(StandardQuadratureRule const &, double targetError,unsigned int maxIter=100);
    //! Copy constructor.
    QuadratureRuleAdaptive(QuadratureRuleAdaptive const &);
    
    //! The clone method.
    /*!  
      The override qualifier hels to avoid error (C++11): this
      method must override a virtual method of the base class.
     */
    virtual QuadratureRuleHandler   clone() const override;
    //!
    /*!@{*/
    int num_nodes()const {return _therule->num_nodes();}
    double node(const int i) const {return _therule->node(i);};
    double weight(const int i)const{return _therule->weight(i);};
    unsigned int order() const{return _therule->order();};
    /*!@}*/
    //! The method that applies the rule.
    virtual double apply(FunPoint const &, double const & a,
			 double const & b) const override;
    virtual ~QuadratureRuleAdaptive(){};
  private:
    double _factor;
    std::unique_ptr<QuadratureRulePlusError> _therule;
    double _targetError;
    unsigned int _maxIter;
  };
}


#endif
