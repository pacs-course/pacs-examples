#ifndef RULES_FACTORY_HPP_
#define RULES_FACTORY_HPP_
#include <map>
#include <vector>
#include <stdexcept>
#include <string>
#include <memory>
#include "numerical_rule.hpp"

namespace NumericalIntegration{
  using std::invalid_argument;
  using std::string;
  using std::auto_ptr;
  /*!
    This is a typedef to the builder
    The bulder returns a pointer to a new object which implements
    a quadrature rule 
   */
  typedef std::auto_ptr<QuadratureRule > (* RuleBuilder)();
  /*! A factory of quadrature rules. It is implemented as a Singleton
    The compulsory way to access a method is RuleFactory::Instance().method()
  */
  class RuleFactory{
  public:
    //! The container for the rules.
    typedef std::map<std::string,RuleBuilder> Container;
    //! Method to access the only instance of the factory
    static RuleFactory & Instance();
    //! Get the rule with given name . The pointer is null if no rule is present.
    std::auto_ptr<QuadratureRule> get(string const & name) const;
    //! Register the given rule
    void set(string const &, RuleBuilder const &) throw(invalid_argument);
    //! Returns a list of registered rules.
    std::vector<string> registered()const;
    //! Unregister a rule
    void unset(string const &);
    ~RuleFactory();
  private:
    RuleFactory();
    RuleFactory(const RuleFactory &);
    RuleFactory & operator =(const RuleFactory &);
    Container _storage;
  };
}
namespace
{
  //! A helper function used only locally.
  //!  It extract the name from the pair returned by the container
  std::string get1(NumericalIntegration::RuleFactory::Container::value_type const &);
}


#endif /* BC_FACTORY1_HPP_ */
