#ifndef H_RULEFACTORY_HH
#define H_RULEFACTORY_HH
#include "Factory.hpp"
#include "QuadratureRuleBase.hpp"
#include <string>
namespace apsc::QuadratureRuleFactory
{
//! Specialization of Factory for QuadratureRules
using RulesFactory =
  GenericFactory::Factory<apsc::NumericalIntegration::QuadratureRuleBase,
                          std::string>;
//! My only factory
extern RulesFactory &MyFactory;
//! A simple method to insert a rule
/*! It is a valid alternative to the "RuleProxy". The RuleProxy us based on the
  utility contained in the Proxy.hpp file (in AbstractFactory directory). It is
  rather nice since it is based on define a special object whose construction
  automatically register a builder in the Factory. However is a bit complicated.
  HAving just a simple template function is easier to understand, even if
  automatic registration will require to use a compiler directive, see in
  myRules.cpp.
*/
template <typename ConcreteRule>
void
addRuleToFactory(std::string const &name)
{
  // get the factory
  RulesFactory &factory = MyFactory;
  factory.add(name, []() {
    return std::unique_ptr<apsc::NumericalIntegration::QuadratureRuleBase>{
      std::make_unique<ConcreteRule>()};
  });
}

} // namespace apsc::QuadratureRuleFactory
#endif
