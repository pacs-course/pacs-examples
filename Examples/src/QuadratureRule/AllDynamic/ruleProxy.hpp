#ifndef H_RULEPROXY_HH
#define H_RULEPROXY_HH
#include <string>
#include "ruleFactory.hpp"
#include "Proxy.hpp"

//!Specialization of Factory and Proxy for QuadratureRules
namespace QuadratureRuleFactory
{
  template <typename ConcreteRule> using 
  RuleProxy=GenericFactory::Proxy<RulesFactory,ConcreteRule>;
}
#endif
