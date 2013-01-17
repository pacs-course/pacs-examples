/*
 * FunHandler.hpp
 *
 *  Created on: Dec 13, 2008
 *      Author: forma
 */

#ifndef RULESPROXY_HPP_
#define RULESPROXY_HPP_
#include <typeinfo>
#include "rulesFactory.hpp"
#include "numerical_rule.hpp"
#include <string>

namespace NumericalIntegration {
  /*! A simple proxy for a quadrature rule.
  It provides the builder and the automatic registration mechanism 
  */
  template<typename T> class RuleProxy {
public:
    //! The constructur dues the registration
	RuleProxy(char const * const &);
	~RuleProxy(){};
    //! The builder
        static auto_ptr<QuadratureRule> Build();
private:
	RuleProxy(RuleProxy const &) {};
	RuleProxy & operator=(RuleProxy const &) {};
};

template<typename T>
auto_ptr<QuadratureRule> RuleProxy<T>::Build() {
	return auto_ptr<QuadratureRule>(new T);
}

template<typename T>
RuleProxy<T>::RuleProxy(char const * const & name) {
  RuleFactory & factory(RuleFactory::Instance());
  factory.set(string(name),RuleProxy<T>::Build);
}

}

#endif /* RULESHANDLER_HPP_ */
