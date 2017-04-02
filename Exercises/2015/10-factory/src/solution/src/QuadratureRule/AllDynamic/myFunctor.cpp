#include "QuadratureRule.hpp"
#include "functionProxy.hpp"
#include "myFunctor.hpp"

// Registration in the factory I use an unnamed namespace
namespace
{
    using NumericalIntegration::FunPoint;
    using namespace FunctionsFactory;

    ProxyFunction FmyFunctor( "functor", FunPoint( myFunctor() ) );

}
