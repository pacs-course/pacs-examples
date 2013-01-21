#ifndef PROXYFUNCTION_HPP_
#define PROXYFUNCTION_HPP_
#include <string>
#include <memory>
#include <type_traits>
#include "QuadratureRule.hpp"
#include "Factory.hpp"

namespace FunctionsFactory
{

  class BuilderFun {

  public:
    typedef NumericalIntegration::FunPoint FunPoint;

    BuilderFun ( FunPoint const & fun ) : M_fun( fun ) {}

    std::unique_ptr<FunPoint> operator () ( ) const
    {
        return std::unique_ptr<FunPoint>( new FunPoint (M_fun) );
    }

  private:

    FunPoint M_fun;

  };


  /*! A simple proxy for registering into a factory.

    It provides the builder as static method
    and the automatic registration mechanism.

    \param Factory The type of the factory.
    \param ConcreteProduct is the derived (concrete) type to be
    registered in the factory
  */
  class ProxyFunction
  {
  public:

    typedef NumericalIntegration::FunPoint FunPoint;

    typedef GenericFactory::Factory<FunPoint, std::string, BuilderFun> Factory_type;

    //! The constructor does the registration.
    ProxyFunction(std::string const & name, FunPoint const & fun)
    {
      // get the factory. First time creates it.
      Factory_type & factory(Factory_type::Instance());

      BuilderFun builder( fun );

      // Insert the builder. The & is not needed.
      factory.add(name, builder);
    }

  private:
    ProxyFunction(ProxyFunction const &)=delete; // only C++11
    ProxyFunction & operator=(ProxyFunction const &)=delete; // only C++11
  };

    }

#endif /* PROXYFUNCTION_HPP_ */
