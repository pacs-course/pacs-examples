#ifndef H_RULEPROXY_HH
#define H_RULEPROXY_HH
#include "Proxy.hpp"
#include "ruleFactory.hpp"
#include <string>

//! Specialization of Factory and Proxy for QuadratureRules
namespace apsc::QuadratureRuleFactory
{
template <typename ConcreteRule>
using RuleProxy = GenericFactory::Proxy<RulesFactory, ConcreteRule>;
}
#endif
