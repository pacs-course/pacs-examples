#include "numerical_rule.hpp"
#include "QuadratureRuleAdaptive.hpp"
#include "ruleFactory.hpp"
//#include "ruleProxy.hpp"
//  NEW SIMPLIFIED VERSION
/*!\file myrules.cpp
  In this file I define the quadrature rules that I want to put in the library that loads them into the Factory/
  
 */
//! Registration in the factory I use an unnamed namespace
/*!
  When I load the library I must be sure that the symbols are loaded
  so I need to use the option -Wl,-E or -Wl,-export-dynamic
 */
/*
namespace TheRules
{
  using namespace NumericalIntegration;
  using QuadratureRuleFactory::RuleProxy;
  
  RuleProxy<Simpson> SH("Simpson");
  
  RuleProxy<Trapezoidal> TH("Trapezoidal");
  
  RuleProxy<MidPoint> MH("MidPoint");
  
  RuleProxy<QuadratureRuleAdaptive<Simpson>> AD("Adaptive");

}
*/
//!Alternative use a function.
/*! You must load the function and run it.
  @code
    void * dylib=dlopen(libraryname,RTLD_NOW);
    void * lfi = dlsym(dylib,"loadFactoryItems")
    static_cast<void(*)()>(lfi)();
  @endcode
  or declare it static with constructor attribute
 */


//! Another alternative. The function is automatically loaded
/*! No need of -Wl,-E linker option */

 __attribute__((constructor))
static void loadFactoryItems()
  {
    using namespace QuadratureRuleFactory;
    using namespace NumericalIntegration;
    addRuleToFactory<Simpson>("Simpson");
    addRuleToFactory<Trapezoidal>("Trapezoidal");
    addRuleToFactory<MidPoint>("MidPoint");    
    addRuleToFactory<QuadratureRuleAdaptive<Simpson>>("Adaptive");
  }

