#ifndef H_RULEPROXY_HH
#define H_RULEPROXY_HH
#include <string>
#include "QuadratureRule.hpp"
#include "Proxy.hpp"
#include "Factory.hpp"
//!Specialization of Factory and Proxy for QuadratureRules
namespace QuadratureRuleFactory
{
  typedef GenericFactory::Factory<NumericalIntegration::QuadratureRule,std::string> RulesFactory;
  //! Only C++11
  template <typename ConcreteRule> using RuleProxy=GenericFactory::Proxy<RulesFactory,ConcreteRule>;
}
#endif
