#ifndef H_FUNCTIONPROXY_HH
#define H_FUNCTIONPROXY_HH
#include <string>
#include "QuadratureRule.hpp"
#include "ProxyFunction.hpp"
#include "Factory.hpp"

//!Specialization of Factory and Proxy for Functions
namespace FunctionsFactory
{
    using NumericalIntegration::FunPoint;

    typedef GenericFactory::Factory < FunPoint, std::string, BuilderFun >
      FunctionsFactory;

}
#endif
