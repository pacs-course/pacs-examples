# Content of the Examples/src directory #

We give here an overview of the content of the `src` directory where all the sources of the examples are stored. All examples have a `README.md` file with more detailed description.

We can divide the examples in four categories:
* **Utilities** Basic utilities meant to be used by other code
* **Basic Examples** They are just illustration of some features of the language
* **Advanced Examples** Still aiming at illustrate features of the language, they focus on particular more numerical, or simply technical, issues
* **Full Examples** Examples where a specific numerical problem is treated in a (relative) depth

## Utilities##
  * `Utilities` A set of utilities that you may use also in your code. All described in the local `README.md` file
  * `GenericFactory`. Copied also in `Utility`, it is a generic object factory. You may use and specialize it for your needs.
  * `muParserInterface` Some examples on how to interface with [MuParser](https://beltoforion.de/article.php?a=muparser) and [MuParseX](https://beltoforion.de/article.php?a=muparserx) library for parsing mathematical expressions. Use them as a guideline if you plan to use these tools. 
  *A copy of the muparser(x) code is available in the sub-folder `Extra`, with all instructions for compilation for the examples of this course*
  * `LinearAlgebra/Utilities` Utilities related to linear algebra and other numerical algorithms (see the local `README.md` file)
  * `LoadLibraries` An utility to hold dynamically loaded  libraries (you have it also in `FactoryPlugin`).
  
## Basic Examples ##
  * `Aggregates` Show some feature of *aggregates*, a particular type of classes that have to satisfy some requirements, see [here for instance](https://en.cppreference.com/w/cpp/language/aggregate_initialization), and allow to very useful constructs
  * `Arrays` Some examples of the use of std::array
  * `Bindings` The binding rule of references and also an example of use of `std::forward` and of the move semantic of `std::vector`. See also `RefBindings`
  * `CRTP` It shows how the Curiosly Recursive Template Pattern works. The example is simple, the feature less simple to understand (but not so difficult after all...).
  * `Constexpr` The use of constexpr functions compared with normal functions. You find here also `HornerConstexpr.hpp`, an implementation of 
  Horner rule (see also `Horner` example) as a constexpr function
  * `ForwardDeclaration` An example that shows a case were a forward declaration is needed
  * `Functors` An xample of use of callable objects.
  * `Horner` The Horner rule to compute polynomials efficiently
  * `IntegerOverflow` An example that shows how integer overflow produces non-intuitive results.
  * `Linkage` Use of `extern` to create variables with external linkage (i.e. global/namespace variable) and unnamed `namespace` and `static` specifiers to have functions with local linkage (i.e. visible only in the current translation unit).
  * `LambdaExpr` Some examples on lambda expressions
  * `MoveSemantic_simple` An illustration of move semantic that shows which constructor/assignment operator is called
  on lvalues and rvalues
  * `Numeric_limits`. It uses almost all the facilities provided by `numeric_limits<>`, just to show what they do.
  * `Odr` An example on the one definition rules and also on a dangerous situation if things are not done correctly.
  * `Polygon`. An example of *polymorphism*: a hierarchy of polygon classes
  * `Preprocessor`. A simple example that shows what happens if you look at a code after having run the preprocessor. Just to show that `include` actually includes and that `-D` sets a preprocessor variable.
  * `RandomDistribution` An example of use of different random number engines and distribution of the standard library. It uses `gnuplot-iostream` to plot histograms on the screen. You must have `gnuplot` installed.
  * `Rational`. An example on operator overloading: a class for rational numbers.
  * `RefBingings` Another example on reference bindings rules where I show all possibilities. See also `Bindings`
  * `ReferenceWrapper` Just to show that thanks to the reference wrapper utility you may store "references" in a container. Normally containers may contain only *first class objects*, not references, but reference wrappers come to a rescue.
  * `Scope` An old example on scoping rule.
  * `SimpleProgram` A simple program that adds two numbers. Just a little more complicated than `Hello World`.
  * `SmartPointers` An example on the use of smart pointers. 
  * `StaticMembers` A possible use of static member variables: count how many objects of a given type you have in the code.
  * `STL/` A folder with examples on various utilities of the STL.
    * `STL/cont` The use of some containers.
    * `STL/FileSystem` The new facility to traverse and examine file systems.
    * `STL/Optional` The new `std::optional` utility to represent *missing data*.
   *  `STL/Reduce` The new utilities `reduce` and `transform_reduce` that merform reduction and transformation operation on ranges possibly in parallel 
    * `STL/Set` The use of set with different comparison operators.
    * `STL/SetEdge` Again the use of comparison operator to distinguish between directed or undirected graphs.
    * `STL/Sort` The use of `std::sort` and comparison operators (again!)
    * `STL/tuple` Some examples on the use of tuples and `std::tie`
    * `STL/Variant` An example of this new utility, a class that may store values of different types. A more type sefe version of `union`.
  * `Streams/` A folder containing several examples of the use of stream, including binary i/o. It also contains in `Streams/Serialization` a first example of serializaion of an aggregate (more complex serialization that may be performed with specialised libraries like `boost::serialization` are not covered here)
  * `Vector` Examples of use of `std::vector`.
## Advanced examples
  * `bcHandler` An example of a possible way to handle the information about boundary conditions in a numerical code. It shows also 
  an example of use of `muparserx`. To install muparserX go in the sub-folder `Extra`
  * `CompositionWithVariadicTemplates` The Composer design pattern done with variadic templates: constructing complex objects by composing simple ones. The technique is more "rigid" than the classical one which uses polymorphism, but rather flexible and efficient.
  * `DesignPatterns/` A set of example of Design Patterns, a C++ implementation of the techniques illustrated in the famous [Gang of Four](https://en.wikipedia.org/wiki/Design_Patterns)book and in [this site.](https://refactoring.guru/design-patterns/cpp)
  * `FloatingPoint` A directory containing several examples on aspects (sometimes degeneracies) of floating point computations, including how to handle floating point exception. See the local `README.md` file.  The example `FloatinPoint\FinDiff` also shows a use of *gnuplot-iostream* to have plots on the terminal.
  * `OptimizationAndProfiling` A set of examples on code optimization and profiling. Only to remember the various tools, which have been integrated in the Makefile. If you read the Makefile you see how tools have been called.
  * `Polynomials`. A more complex examples of a class that implements the generic concept of a polynomial. An example of generic programming.
  * `MetaProgramming/`. A folder with examples that show  metaprogramming techniques.
    * `DelcTypeDeclval` An example of the use of the decltype/declval cinstruct. In fact, it contains also examples of some type_traits.
    * `Enable_if` It illustrate a possible use of `std::enable_if` type-trait of the Standard Library.
    * `GCD` A useless examples on how to compute the greater common divisor statically. Different versions are provided. Now you have it in the
    standard library (and surealy a better implementation than mine)
    * `ifthenelse` Poor-man implementation of the `std::conditional` type-trait. 
    * `IsClonable` An utility (a better version in `Utility/ColingUtilities.hpp` file) to test if a class contains the `clone()` member function.
    * `MetaDot`. Dot product of arrays computed statically. Different versions are provided.
    * `Pi`. Pi computed  by computing a truncated series with a recursive technique.
    * `Pow`. Implementation of pow for integer exponents. Different version provided (but recent implementation of `std::pow` are now rather efficient so the example is of no practical use. Even if I still suggest to use `x*x` instead of `std::pow(x,2)`.
    * `SwitchType`. The use of type alias to allow mesh elements dedice the type of their buonday depending on the space dimensions.
    * `trasposeView` An overly complex example of a transpose view of a matrix, To show a possible use of `std::enable_if` (but I also provide an alternative).
    * `Trivial` USe of type-traits to detect Trivial, Trivially-Constructable and StandardLayout types. With an example on how Trivially-Constructable and StandardLayout property may be useful.
    * `TypeTraitsC11` A set of programs that show some type-traits.
  * `MoveSemantic` A examples that uses [valgrind](https://valgrind.org/ "The valgrind site") to show the advantages of move semantic when handling large objects (you must have valgrind installed in your system).
  * `NonLinSys` A class that holds a system of callable objects, where you can add the objects one at a time. It also contains a `FunctionFactory`, a factory of
  callable objects. The latter may be used in a program where functions are given by the user and then used throughout the program, for instance functiosn that define boundary conditions.
  * `Parallel` Some examples of parallelization: in `Parallel/MPI` and `Parallel/OpenMP`
  * `Plugins` Two examples of plugin architecture
  * `ReadingVector` This is an example on how you can imitate the stange way used by Eigen to read a vector from a stream by overloading the
  streaming and the comma operator. More a curiosity then something of practical use.  
  * `SharedLibrary` An example of set up of shared libraries with version control. And in `SharedLibrary/DynamicLoading` how to lead functions
  dynamically from a shared library
  * `Templates/` A set of examples on the use of templates. Some simple, some less simple:
    - `Compare` An example of the use of policies (Bridge design pattern) as template parameter to change the behaviour of comparison operations
    - `ExplicitInstatiation` An example of explicit instantiation of templates. A technique to save compilation time of template instances that you use often.
    - `Fibonacci` A classic example of template metaprogramming: a recursive template 
    - `Interpolation` Polynomial interpolation on a set of points (x,y). Use of policy to choose from different interpolation strategies. *Note* It differs from the code contained in the `Interp1D` folder: the latter implements just piecewise linear interpolation but in a very generic fashion.
    - `Mean` An example of use of tempalte specialization or variadic templates to create functions that computes sumes and average of values. 
    - `traits` Use of traits/policies to create an inner product able to operate seemlessly on vectos of floating points or of complex numbers. 
    - `VariadicTemplates` An illustration of variadic templates
    

## Full examples

  * `bcHandler` An example on handling different boundary conditions. It is just an outline that must be completed to be fully operative.
  * `Derivatives` An example of using recursive templates to compute finite difference to approximate derivatives of any order. 
  * `ExpressionTemplates` Two implementations of Expression Templates, sufficient to understand how they work.
  * `FixedPointSolver` Utilities for solving x=F(x) via fixed point iteration. It allows the implementation of accelerators of convergence, passed as policy. I have implemented one. A complete and working example.
  * `LineSearch`. Minimization of functions using line search type algorithms
  * `HeatExchange` A simple finite element code that solves a 1D problem. An example also of use of *GetPot* and *GnuPlot*
  * `Interp1D` A generic code for piecewise linear interpolation. Implemented in a very general way to allow different specializations according to the needs.
  * `LinearAlgebra` Various tools for LinearAlgebra (and not only)
    * `CppNumericalSolvers` A submodule containing a fork of the homonymous  library [on github.](https://github.com/PatWie/CppNumericalSolvers)
    * `IML_Eigen` A porting to the [Eigen library](http://eigen.tuxfamily.org/index.php?title=Main_Page) of the algorithms for solution of sparse linear systems of equations described in [templates for the solution of linear systems](https://www.netlib.org/linalg/html_templates/Templates.html).
    * `IML_Eigen/SaddlePointSolver` A complete code for the solution of a saddle point problem arising from the disctretization of a hybrid dimensional 
    Darcy problem. You find the detail of the problem in [Preconditioning Techniques for the Numerical Solution of Flow in Fractured Porous Media](https://doi.org/10.1007/s10915-020-01372-0)
    * `MatrixData` Some matrices taken from [Matrix Market](https://math.nist.gov/MatrixMarket/), a repo of data for comparative studies of algorithms.
    * `MatrixMarketReadersMatlab` If you waant to load the matrices into Matlab
    * `redsvd-h` A submodule containing the fork of the homonymous tool [on Github](https://github.com/ntessore/redsvd-h), which implements
    in a templated fashion the algorithms in [redsvd](https://code.google.com/archive/p/redsvd/), for solving several matrix decompositions including singular value decomposition (SVD), principal component analysis (PCA), and eigenvalue decomposition. It uses a stocastic technique to be able to extract low rank approximations of possibly very large sparse matricies at low cost. See [Finding Structure with Randomness: Probabilistic Algorithms for Constructing Approximate Matrix Decompositions](https://doi.org/10.1137/090771806)
    
    * `spectra` A submodule containing a fork of the homonymous tool on [GitHub](https://github.com/yixuan/spectra.git). The name is the acronim of Sparse Eigenvalue Computation Toolkit as a Redesigned ARPACK. It is a C++ library for large scale eigenvalue problems, built on top of Eigen linear algebra library
    * `Thomas` An implementation of the Thomas algoritm to solve tridiagonal systems of equations. It is a well known O(n) algorithm.
    You find the general desctiption on [Wikipedia](https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm)
    * `SparseBlockMatrix` A tool to construct block matrices where each block is a sparse Eigen matrix.
    * `Utilities` Described in the *Utilities* section.
  * `Mesh`, An example of a class for 2D meshes, with a few utilities.
  * `MyMat0` Example of a class for Matrices with different implementation and some views.
  * `Multicity` It uses `RKFSolver` to implement a multicity epidemic model.
  * `NewtonSolver` Solving F(x)=0. It supports Newton but also quasi-Newton shemes. Indeed it is very general. **Note** The use of a `callbak`, a technique to allow getting information on the internal state of a class, when needed. A complete and working example.
  * `OneDMesh`. A class for 1D meshes. Build with a structure that wants to mimic what you may have also in 2 o 3D meshes. It also implements an algorithm for non-uniform meshes, by specifying a spacing function.
  * `QuadratureRule/` Classes and techniques for numerical integration of univariate function. Offers also  exampled of dynamic loading of polymorphism, clonable classes, dynamic loading of libraries, decorator design pattern, templates. A complete and working example.
  * `Regression`. An example of univariate linear regression. You can chose different polynomial models. At the moment only least square fitting is implemented.
  * `RKFSolver` Class template for explicit Runge Kutta embedded schemes for scalar ordinary differential equation (Cauchy problem). Support scalar, vectorial and matricial systems. Highly expandable.  A complete and working example.
    
  
  


