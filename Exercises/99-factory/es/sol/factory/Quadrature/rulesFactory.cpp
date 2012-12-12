/*
 * bc_factory1.cpp
 *
 *  Created on: Nov 28, 2008
 *      Author: forma
 */
#include "rulesFactory.hpp"
#include <algorithm>
#include <iterator>

namespace NumericalIntegration {

RuleFactory & RuleFactory::Instance() {
	static RuleFactory theFactory;
	return theFactory;
}

auto_ptr<QuadratureRule> RuleFactory::get(const string & name) const {
	Container::const_iterator f = _storage.find(name);
	return (f == _storage.end()) ? auto_ptr<QuadratureRule>(): f->second();
}

void RuleFactory::set(const string & name, RuleBuilder const & func)
		throw(invalid_argument) {
	std::pair<Container::iterator, bool> f = _storage.insert(std::make_pair(name, func));
	if (f.second == false)
		throw invalid_argument("double registration in RuleFactory");
}

void RuleFactory::unset(const string & name) {
	_storage.erase(name);
}

std::vector<string> RuleFactory::registered() const {
  std::vector<string> tmp(_storage.size());
  std::transform(_storage.begin(),_storage.end(),tmp.begin(),get1);
	return tmp;
}

RuleFactory::RuleFactory() {
}

RuleFactory::RuleFactory(const RuleFactory & RuleFactory) {
}

RuleFactory & RuleFactory::operator =(const RuleFactory & RuleFactory) {
	return *this;
}

RuleFactory::~RuleFactory() {
}

}
namespace{
  std::string get1(NumericalIntegration::RuleFactory::Container::value_type const & c){return c.first;}
}





