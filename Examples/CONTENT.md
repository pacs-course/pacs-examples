# Content of the Examples directory #

All examples have a `README.md` file with more detailed description

## Basic Examples ##
  * `Array` The use of std::array and uniform (brace) initialization
  * `Bindings` The binding rule of references and also an example of use of std::forward and move semantic of std::vector
  * `Constexpr` The use of constexpr functions compared with normal functions. You find also here `HornerConstexpr.hpp`, an implementation of 
  Horner rule (see [[Horner]]) as a constexpr function
  * `Findiff` A code that shows how finite differences may be prone to round-off error. An example also for use of *Gnuplot*.
  * `FloatingPointFailures` More a curiosity than an example, it shows how sometimes roundoff error may lead to wrong answers. 
  * `ForwardDeclaration` An example that shows a case were a forward declaration is needed
  * `FPComparison` How comparing two floating points may be difficult because of roundoff errors
  * `FPExceptions` Tools for control floating point exceptions. You may use them freely if you want.
  * `Functors` An example of use of callable objects.
  * `HeatExchange` A simple finite element code that solves a 1D problem. An example also of use of *GetPot* and *GnuPlot*
  * `Horner` The Horner rule to compute polynomials efficiently
  * `InitializationList` Examples on the uniform initialization, also called braced initialization, in different contexts.
  * `IntegerOverflow` An example that shows how integer overflow produces non-intuitive results.
  * `Linkage` Use of `extern` to create variables with external linkage (i.e. global/namespace variable) and unnamed `namespace` and `static` specifiers to have functions with local linkage (i.e. visible only in the current translation unit).
  * `MoveSemantic` A basic examples that shows the advantages of move semantic when handling large objects (it uses valgrind to assess memory usage). 
  * `Numeric_limits`. It uses almost all the facilities provided by numeric_limits, just to show what they do.
  * `Odr` An example on the one definition rules and also on a dangerous situation if things are not done correctly.
  * `OptimizationAndProfiling` A set of examples on code optimization and profiling. Only to remember the various tools, which have been integrated in the Makefile. If you read the Makefile you see how tools have been called.
  * `Parallel` The use of some parallel standard algorithms.
  * `Polygon`. An example of *polymorphism*: a hierarchy of polygon classes
  * `Polynomials`. A more complex examples of a class that implements the generic concept of a polynomial. An example of generic programming.
  * `Preprocessor`. A simple example that shows what happens if you look at a code after having run the preprocessor. Just to show that `include` actually includes and that `-D` sets a preprocessor variable.
  * `QuadraticRoot` Another example of possible error given by roundoff. It is more evident if you use the `float` version, since floats have less significant digits than doubles.
  * `RandomDistribution` An example of use of different random number engines and distribution of the standard library.
  * `Rational`. An example on operator overloading: a class for rational numbers.
  * `ReferenceWrapper` Just to show that thanks to the reference wrapper utility you may store "references" in a container. Normally containers may contain only "first class" objects, not references, but referencce wrapper comes to a rescue.
  * `Scope` An old example on scoping rule.
  * `SharedLibrary` An example of set up of shared libraries with version control.
  * `SimpleProgram` A simple program that adds two numbers. Just a little more complicated than `Hello World`.
  * `SmartPointers` An example on the use of smart pointers. 
  * `StaticMembers` A possible use of static member variables: count how many objects of a given type you have in the code.
  * `STL/` A folder with examples on various ulilities of the STL.
    * `STL/cont` The use of some containers.
    * `STL/FileSystem` The new facility to traverse and exhamine file systems.
    * `STL/Set` The use of set with different comparison operators.
    * `STL/SeEdge` Again the use of comparison operator to distinguish between directed or undirected graphs.
    * `STL/tuple` Some examples on the use of tuples.
    * `STL/optional` The new `std::optional` utility to represent *missing data*.
    * `STL/Variant` An example of this new utility, a class that may store values of different types. A more type sefe version of `union`.
    * `STL/Sort` The use of `std::sort` and comparison operators (again!).
  * `Streams/` A folder containing examples of the use of stream, including binary i/o.
  * `Thomas` An algorithm to solve tridiagonal systems of equations. **The version for symmetric condition is buggy**
  * `Vector` Examples of use of `std::vector`.
  
## Utilities

  * `Utilities` A set of utilities that you may use also in your code. All descrived in the `README.md` file
  * `GenericFactory`. Copied also in `Utility`, it is a generic object factory. You may use and specialize it for your needs.
  * `muParserInterface` Some examples on how to interface with [MuParser](https://beltoforion.de/article.php?a=muparser) and [MuParseX](https://beltoforion.de/article.php?a=muparserx) library for parsing mathematical expressions. Use them as a guideline if you plan to use these tools. 

**Note**: Of course you may hacker any tool in the course Example repo if you need it for your project!

## More complex examples

  * `Accumulate` An example that shows different techniques for summing vectors containing a large number of values of different orders of magnitute and different sign. This is a situation where roundoff error may easily accumulate and make the rusult inaccurate.
  * `bcHandler` An example on handling different boundary conditions. It is just an outline that must be completed to be fully operative.
  * `CompositionWithVariadicTemplates` A simple example of a complex C++ feature: variadic templates. Here used to allow to compose more complex objects from simple components, by inheriting from the tamplete arguments. With the addition of some C++17 features it can be made even more complete.
  * `CRTP` It shows how the CRTP works. The example is simple, the feature less simple to understand (but not so difficult after all...).
  * `Derivatives` An example of using recursive templates to compute finite difference to approximate derivatives of any order. 
  * `DesignPatterns/` A set of example of Design Patterns, a C++ implementation of the techniques illustrated in the famous [Gang of Four](https://en.wikipedia.org/wiki/Design_Patterns)book.
  * `ExpressionTemplates` My poor-man implementation of Expression Templates, sufficient though to understand how they work.
  * `FactoryPlugin` and `FactoryPluginII`. Object factories used to implement a `plugin-like` architecture. 
  * `FixedPointSolver` Utilities for solving x=F(x) via fixed point iteration. It allows the imlplementation of accelerators of convergence, passed as policy. I have implemented one. A complete and working example.
  * `Interp1D` A generic code for piecewise linear interpolation. Implemented in a very general way to allow different personalizations according to the needs.
  * `LinearAlgebra` Various tools for LinearAlgebra (and not only): iterative methods for linera system of equations, computation of eigenvalues/eigenvector, reduced rank approximation, tools for reading matrices in MatrixMarked format, optimization (of a non-linear function of several variable). They rely on external libraries (some of which provided directly as git submodules), read the `README` file. You can use them, it's open source software.
  * `LoadLibraries` An example of dynamic loading of libraries (you have if also in `FactoryPlugin`.
  * `Mesh`, An example of a class for 2D meshes, with a few utilities.
  * `MetaProgramming/`. A folder with examples that show  metaprogramming techniques.
    * `DelcTypeDeclval` An example of the use of the decltype/declval cinstruct. In fact, it contains also examples of some type_traits.
    * `Enable_if` It illustrate a possible use of `std::enable_if` type-trait of the Standard Library.
    * `GCD` A useless examples on how to compute the greater common divisor statically. Different versions are provided.
    * `ifthenelse` Poor-man implementation of the `std::conditional` type-trait.
    * `IsClonable` An utility (contained also in the `Utility` folder) to test if a class contains the `clone()` member function.
    * `MetaDot`. Dot product of arrays computed statically. Different versions are provided.
    * `Pi`. Pi computed with a recursive technique by computing a truncated series.
    * `Pow`. Implementation of pow for integer exponents. Different version provided (but recent implementation of `std::pow` are now rather efficient so the example is of no practical use. Even if I still suggest to use `x*x` instead of `std::pow(x,2)`.
    * `SwitchType`. The use of type alias to allow mesh elements dedice the type of their buonday depending on the space dimensions.
    * `trasposeView` An overly complex example of a transpose view of a matrix, To show a possible use of `std::enable_if` (but I also provide an alternative).
    * `Trivial` USe of type-traits to detect Trivial, Trivially-Constructable and StandardLayout types. With an example on how Trivially-Constructable and StandardLayout property may be useful.
    * `TypeTraitsC11` A set of programs that show some type-traits.
  * `MyMat0_XX` Example of a class for Matrices with different implementation (the most interesting is the template one) and some views.
  * `NewtonSolver` Solving F(x)=0. It supports Newton but also quasi-Newton shemes. Indeed it is very general. **Note** The use of a `callbak`, a technique to allow getting information on the internal state of a class, when needed. A complete and working example.
  * `OneDMesh`. A class for 1D meshes. Build with a structure that wants to mimic what you may have also in 2 o 3D meshes. It also implements an algorithm for non-uniform meshes, by specifying a spacing function.
  * `QuadratureRule/` Classes and techniques for numerical integration of univariate function. Offers also  exampled of dynamic loading of polymorphism, clonable classes, dynamic loading of libraries, decorator design pattern, templates. A complete and working example.
  * `Regression`. An example of univariate regression. You can chose different polynomial models. There is also a video on YouTube.
  * `RK45`. Old version of a Runge Kutta adaptive solver for scalar ordinary differential equation (Cauchy problem). Superseded by `RKFSolver`. It has been left only becouse used in `OneDMesh` (and consequently `QuadratureRule`).
  * `RKFSolver` Class template for Runge Kutta embedded schemes for scalar ordinary differential equation (Cauchy problem). Support scalar, vectorial and matricial systems. Highly expandable.  A complete and working example.
    
  
  


