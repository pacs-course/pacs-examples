#ifndef H_RULEPROXY_HH
#define H_RULEPROXY_HH
#include <string>
#include "QuadratureRule.hpp"
#include "QuadratureRuleAdaptive.hpp"
#include "Proxy.hpp"
#include "Factory.hpp"
//!Specialization of Factory and Proxy for QuadratureRules
namespace QuadratureRuleFactory
{
  typedef GenericFactory::Factory<NumericalIntegration::QuadratureRule,std::string> RulesFactory;
  //! Only C++11
  template <typename ConcreteRule> using RuleProxy=GenericFactory::Proxy<RulesFactory,ConcreteRule>;
}

//! If I am using adaptive quadrature rule I need to set tolerance and iteration
template< typename RULE>
struct ruleWrapper
{
  static void setTargetError(RULE & r, double t){};
  static void setMaxIter(RULE &, unsigned int n){};
};

//! Specialization 
template< typename RULE>
struct ruleWrapper<NumericalIntegration::QuadratureRuleAdaptive<RULE>>
{
  static void setTargetError( NumericalIntegration::QuadratureRule& r, double const t)
  {
    dynamic_cast<NumericalIntegration::QuadratureRuleAdaptive<RULE>&>(r).setTargetError(t);
  };
  static void setMaxIter(NumericalIntegration::QuadratureRule & r, unsigned int n)
  {
    dynamic_cast<NumericalIntegration::QuadratureRuleAdaptive<RULE>&>(r).setMaxIter(n);
  };
};

#endif
