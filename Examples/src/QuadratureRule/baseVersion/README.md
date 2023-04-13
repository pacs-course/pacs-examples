# Basic classes and function for numerical integration #

The code contained in this directory produces the basic libary for the
example on numerical integration seen at lecture.

In particular:

* `QuadratureRuleTraits.hpp` The basic types used throughout the code;

* `QuadratureRuleBase.hpp` Contains the declaration of the base class `QuadratureRuleBase`, needed to all files that creates a concrete quadature rule. It is a header only file that contains the code for the basis (abstract) class of standard quadrature rules `QuadratureRule`. It must be included by any file that defines a quadrature rule

* `StandardQuadratureRule.hpp` It contains the base class `StandardQuadratureRule`, derived from `QuadratureRule`, of all the standard quadrature rules.

* `Adams_rule.hpp|cpp` and `Gauss_rule.hpp` Contain the definition of some concrete standard quadrature rule.

* `QuadratureRuleWithError.hpp` is a header only file that contains a
  decorator to implement a standard quadrature rule with the
  computation of an estimate of the error. It decorates `StandardQuadratureRule`,

* `QuadratureRuleAdaptive.hpp` is an header only file that containes a decorator of  `StandardQuadratureRule` to implement adaptive numerical integration

* `montecarlo.hpp` Defines a decorator of `QuadratureRule` that implements Montecarlo integration.

* `numerical_integration.hpp` contains the definition of the class
  that implements composite quadrature rule. It uses a quadrature rule
  as policy to apply  a concrete rule on each sub-interval. It impements the Strategy design pattern.

* `libquadrature` is the library that stores the code for composite quadrature rule. It depends on the library `libMesh1D`, and on GetPot so you have first
to to `OneDMesh` and  look carefully at the README files to install it (it depends on other libraries as well).

* `libintegrands` is a library that stores some possible integrands

To do everything, including the test program:

    make distclean
    make alllibs (maybe with DEBUG=no)
    make install
    
You may also have a look at `main.integration.cpp`,which stores the test program, and try to run it.

# What do I learn here?  #
- An example of polymorphism. All quadrature rules derive from a common base
- An example of the **Decorator design pattern**, look [here](https://refactoring.guru/design-patterns/cpp) if you are interested in design patterns. Indeed, some rules are made by decorating existing classes. The Decorator patter is useful
when you want to extend the capabilities of an existing hyerarchy of classes, uniformly over all of them; 
- Another example of use of helper objects to enucleate a task, in this case reading parameters from a getpot file. When you have a well defined task enucleate it in an object (a class or a function). You will be able to verify the code separately, and reuse the it in other contexts more easily;

