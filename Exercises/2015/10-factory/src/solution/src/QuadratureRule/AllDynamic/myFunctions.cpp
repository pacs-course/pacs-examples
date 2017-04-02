#include "QuadratureRule.hpp"
#include "functionProxy.hpp"
#include "udf.hpp"

// Registration in the factory I use an unnamed namespace
namespace
{
    using NumericalIntegration::FunPoint;
    using namespace FunctionsFactory;

    ProxyFunction FSinCos( "fsincos", FunPoint( fsincos ) );

    ProxyFunction FPippo( "pippo", FunPoint( pippo ) );

    ProxyFunction FPluto( "pluto", FunPoint( pluto ) );

    ProxyFunction FP( "myfun", FunPoint( myfun ) );
}
