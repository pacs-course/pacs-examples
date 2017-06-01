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
  using RulesFactory= GenericFactory::Factory
    <
    NumericalIntegration::QuadratureRule,std::string
    >;
  //! Only C++11
  template <typename ConcreteRule> using 
  RuleProxy=GenericFactory::Proxy<RulesFactory,ConcreteRule>;
}
#endif
