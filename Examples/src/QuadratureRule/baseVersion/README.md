#Basic classes and function for numerical integration#

The code contained in this directory produces the basic libary for the
example on numerical integration seen at lecture.

In particular:

* `QuadratureRuleBase.hpp` Contains the declaration of the base class `Quadrule`, needed to all files that creates a concrete quadature rule


* `QuadratureRuleBase.hpp` is a header only file that contains the code for the basis (abstract) class of standard quadrature rules `QuadratureRule`. It must be included by any file that defines a quadrature rule

* `QuadratureRule.hpp` It contains the base class `StandardQuadratureRule`, derived from `QuadratureRule`, of all the standard quadrature rules.

* `numerical_rule.hpp` Contains the declaration of some concrete standard quadrature rule.

* `QuadratureRuleWithError.hpp` is a header only file that contains a
  decorator to implement a standard quadrature rule with the
  computation of an estimate of the error. It decorates `StandardQuadratureRule`,

* `QuadratureRuleAdaptive.hpp` is an header only file that containes a decorator of  `StandardQuadratureRule` to implement adaptive numerical integration

* `montecarlo.hpp` Defines the decorator of `QuadratureRule` that implements Montecarlo integration.

* `numerical_integration.hpp` contains the definition of the class
  that implements composite quadrature rule. It uses a
  `QuadratureRule` as policy to apply  a concrete rule on each
  sub-interval.

* `libquadrature` is the library that stores the code for composite quadrature rule. It depends on the library `libMesh1D`, and on GetPot so you have first
to go to src/Utilities and then to src/OneDMesh, look at the README
    files and remember to do `make install` in each directory.

* `libintegrands` is a library that stores some possible integrands

To do everything, including the test program:

    make distclean
    make alllibs (maybe with DEBUG=no)
    make install
    
You may also have a look at `main.integration.cpp`,which stores the
test program, and try to run it.
