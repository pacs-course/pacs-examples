# Content of the Examples/src directory #

We provide an overview of the content in the `src` directory, where all the example sources are stored. *All examples have a `README.md` file with a more detailed description*.

We can divide the examples into four categories:

- **Utilities** Basic utilities meant to be used by other code; **Please read this section first and execute the commands indicated, in the given order** 
- **Basic Examples** They are just illustrations of some features of the language;
- **Advanced Examples** Still aiming at illustrate features of the language, they also focus on particular numerical, or simply technical, issues;
- **Full Examples** Examples where a specific numerical problem is treated in a (relative) depth;

In the following, the name of the example is the name of the directory where the code is stored. The description is very brief, just to give an idea of what the example is about. For more details, please refer to the `README.md` file located in the example directory.

## Utilities ##
There are two folders with general utilities. Since they are used by other examples you should first install them as indicated in the following:

  - `Utilities` A set of utilities that you may also use in your code. All is described in the local `README.md` file. 
  **You should go into the `Utilities` directory and type**

```
    make
    make install
```

since several examples make use of the utilities.
  
  - `LinearAlgebraUtil` A second set of utilities more specialised for linear algebra (but not only). Have a look, there are several goodies. They are all described in the local `README.md` file.  **You should go into the directory and type**

```
    make
    make install
```

since several examples use the utilities contained in this folder.

- `GenericFactory`. Copied also in `Utility`, it is a generic object factory. You may use and specialize it for your needs.
- `muParserInterface` Some examples on how to interface with [MuParser](https://beltoforion.de/article.php?a=muparser) and [MuParseX](https://beltoforion.de/article.php?a=muparserx) libraries
  for parsing mathematical expressions. Use them as a guideline if you plan to use these tools. The source of the libraries is contained in the `Extra` folder under the root directory of this repo: you need to install them before being able to compile these examples. Go in the `Extra` folder and follow the instructions in the `README.md` file.
- `LoadLibraries` A utility to hold dynamically loaded  libraries (you have it also in `FactoryPlugin`). It simplifies recalling all the tools needed to access dynamic libraries, and it implements the RAII principle: when destroyed, it unloads the libraries.
  
## Basic Examples ##
  * `Aggregates` Show some feature of *aggregates*, a particular type of classes that have to satisfy some requirements, see [here for instance](https://en.cppreference.com/w/cpp/language/aggregate_initialization), and allow very useful constructs.
  * `Arrays` Some examples of the use of std::array
  * `Bindings` The binding rule of references and also an example of use of `std::forward` and of the move semantics of `std::vector`. See also `RefBindings` on the same topic.
  * `Constexpr` The use of constexpr functions compared with regular functions. You also find here `HornerConstexpr.hpp`, an implementation of 
  Horner rule for polynomial evaluation (see also `Horner` example) as a constexpr function.
  * `Concepts` A brief explanation of concepts, a new feature introduced in C++20. With some examples of the creation of user-defined concepts. See [here](https://www.modernescpp.com/index.php/c-20-concepts-the-placeholder-syntax/) for a general introduction.
  * `CRTP` It shows how the Curiously Recursive Template Pattern works. The example is simple, the feature is less simple to understand (but not so difficult after all...). It may be useful to understand the implementation of the [Expression Templates](https://en.wikipedia.org/wiki/Expression_templates) technique.
  * `ForwardDeclaration`: An example that shows a case where a forward declaration is needed.
  * `Functors`: An example of the use of callable objects (functors). It also shows how to use `std::function` to store callable objects in a container.
  * `Horner` The Horner rule to compute polynomials efficiently, in a serial environment at least, the code is inherently scalar.
  * `IntegerOverflow` An example that shows how integer overflow produces non-intuitive results. *Beware of integer overflows*. In `Ulilities/absdiff.hpp` you find a function that computes the absolute difference of two integers, eliminating the risk of overflow
  * if the integers are unsigned.
  * `LambdaExpr` Some examples of lambda expressions. Lambda expressions are a very powerful tool, they are the equivalent of function handles in Matlab.
  * `Linkage` Use of `extern` to create variables with external linkage (i.e. global/namespace variable) and unnamed `namespace` and `static` specifiers to have functions with local linkage (i.e. visible only in the current translation unit). A bit of technical stuff.
  * `MoveSemantic_simple` An illustration of move semantics that shows which constructor/assignment operator is called
  on l-values and r-values.
  * `Numeric_limits`. An example that uses almost all the facilities provided by `numeric_limits<>`, just to show what they do.
  * `Odr` An example on the one definition rules and also on a dangerous situation if things are not done correctly. A bit technical stuff.
  * `OrderingAndSpaceShip` An example of the ordering relation concepts introduced in C++20. It also shows how to use the spaceship operator and defaulted comparison operators.
  * `Polygon`. A classic example of *polymorphism*: a hierarchy of polygon classes.
  * `Preprocessor`. A simple example that shows what happens if you look at a code after having run the preprocessor. Just to show that `#include` does what it says and that `-D` sets a preprocessor variable.
  * `RandomDistribution` An example of use of different random number engines and distribution of the standard library. It uses `gnuplot-iostream` to plot histograms on the screen. You must have `gnuplot` installed and also some boost libraries. `gnuplot-iostream` is provided with the Utilities.
  * `Rational`. An example on operator overloading: a class for rational numbers.
  * `RefBindings` Another example of reference bindings rules, where I show all the possibilities of reference bindings. See also `Bindings`.
  * `ReferenceWrapper` Just to show that thanks to the reference wrapper utility, you may store "references" in a container. Typically, containers may contain only first-class objects, not references; however, reference wrappers come to the rescue.
  * `Scope`: An old example of scoping rules.
  * `SimpleProgram` A simple program that adds two numbers. Just a little more complicated than `Hello World`.
  * `SmartPointers`: An example of the use of smart pointers. 
  * `StaticMembers` A possible use of static member variables: count how many objects of a given type you have in the code.
  * `STL/` A folder with examples on various utilities of the STL, and some tricks! .
    - `STL/cont` The use of some containers.
    - `STL/FileSystem` The new facility to traverse and examine file systems.
    - `STL/Map` The powerful C++ associative container.
    - `STL/Optional` The new `std::optional` utility to represent *missing data*.
    - `STL/RangesAndViews` An example of the use of ranges and views, introduced in c++20. It also contains example of the use of the new *contrained algorithms*: more convenient to use and more powerful than the usual ones.
    - `STL/Reduce` The new utilities `reduce` and `transform_reduce` that perform reduction and transformation operations on ranges, possibly in parallel 
    - `STL/Set` The use of set with different comparison operators.
    - `STL/SetEdge` Again, the use of a comparison operator to distinguish between directed or undirected graphs.
    - `STL/Sort` The use of `std::sort` and comparison operators (again!).
    - `STL/SortAndPermute` Sometimes you want to sort a container and then perform the same permutation of elements of other containers. Here a way of doing it.
    - `STL/tuple` Some examples on the use of tuples and `std::tie`
    - `STL/UnorderedMap` The powerful C++ associative container. The unordered version. Is the C++ version of the Python dictionary.
    - `STL/Variant` An example of this new utility, a class that may store values of different types. A safer version of `union`.
  * `Streams/` A folder containing several examples of the use of stream, 
    * `Streams/binary_io`. An example of binary i/o that shows how faster it is on large data set! It shows also an example of the use of HDF (you need hdf installed).
    * `Streams/fstream`. Streaming a file (basic stuff).
    * `Streams/iostream`. Basic streaming operations.
    * `Streams/redirect`. Streams can be redirected, even at runtime. Useful if you want to switch from screen to file, or to a string.
    * `Streams/serialization`. An example of serialization of an aggregate (more complex serialization that may be performed with specialised libraries like `boost::serialization` is not covered here).
    * `Streams/sstream`. String streams are a handy tool. They are strings that can act as a stream (or vice versa). Here you find some examples of usage.
    * `Streams/format` An example of the use of the new `std::format` utility. It is a new feature in C++20 that allows you to format strings in a more readable and maintainable way. It is similar to `printf` but with a more modern syntax. 
  * `Vector` Examples of use of `std::vector`, probably the most used container in C++.
    *`Vector/Basics` As the name says.
    *`Vector/Remove` What it means "removing" elements from a vector with `remove()`. It is not what you think!`
  
## Advanced examples
  * `adtTree` A binary search structure based on the alternating digital tree algorithm. It can search for "points inside boxes"
  and "possibly intersecting boxes". It is a pretty dynamic structure where nodes (boxes or points) can be added/deleted dynamically. A very complex example.
  * `bcHandler` An example of a possible way to handle the information about boundary conditions in a numerical code. It also shows 
  an example of use of `muparserx`. To install `muparserX`, go in the sub-folder `Extra`.
  * `CompositionWithVariadicTemplates` The Composer design pattern done with variadic templates: constructing complex objects by composing simple ones. The technique is more "rigid" than the classical one, which uses polymorphism, but somewhat flexible and efficient. 
  * `DataHeap` A class that implements a heap structure operating on a vector without moving the vector elements around. It is a very efficient implementation of a heap.
  * `DesignPatterns/` A set of examples of Design Patterns, a C++ implementation of the techniques illustrated in the famous [Gang of Four](https://en.wikipedia.org/wiki/Design_Patterns)book and in [this site](https://refactoring.guru/design-patterns/cpp). 
  * `FloatingPoint/` A directory containing several examples on aspects (sometimes degeneracies) of floating point computations, including how to handle floating point exceptions. See the local `README.md` file.  The example `FloatinPoint\FinDiff` also shows another use of *gnuplot-iostream* to have plots on the terminal.
  * `MetaProgramming/`. A folder with examples that show metaprogramming techniques (note, since C++20, some of them are obsolete, yet still working).
    - `DelcTypeDeclval` An example of the use of the decltype/declval construct. In fact, it also contains examples of some type_traits.
    - `GCD` A useless example on how to compute the greatest common divisor statically. Different versions are provided. Now you have it in the
    standard library (and surely a better implementation than mine).
    - `ifthenelse` Poor-man implementation of the `std::conditional` type-trait. 
    - `IsCloneable`: A utility (a better version is contained in `Utility/CloingUtilities.hpp`) to test if a class contains the `clone()` member function.
    - `MetaDot`. Dot product of arrays computed statically. Different versions are provided.
    - `Pi`. Pi is computed  by computing a truncated series with a recursive technique.
    - `SwitchType`. The use of a type alias allows mesh elements to deduce the type of their boundary depending on the space dimensions.
    - `transposeView` An overly complex example of a transpose view of a matrix, to show a possible use of `std::enable_if` (but I also provide an alternative).
    - `Trivial` USe of type-traits to detect *Trivial*, *Trivially-Constructable* and *StandardLayout* types. With an example of how the Trivially-Constructable and StandardLayout properties may be useful.
  * `Mesh` example of tools to store and read a 2D mesh of polygons (currently implemented for triangles and quads).
  * `MoveSemantic` A example that uses [valgrind](https://valgrind.org/ "The valgrind site") to show the advantages of move semantics when handling large objects (you must have valgrind installed in your system).
  * `NonLinSys` A class that holds a system of callable objects, where you can add the objects one at a time. It also contains a `FunctionFactory`, a factory of
  callable objects. The latter may be used in a program where the user gives functions and then uses them throughout the program, for instance, functions that define boundary conditions.
  A different Function Factory is present in `Factory` as a specialization of the generic factory contained therein.
  * `OptimizationAndProfiling/` A set of examples on code optimization and profiling. Various tools have been integrated into the Makefile. If you read the Makefile, you see how tools are called.
  * `Parallel` Some examples of parallelization
    - `Parallel/MPI` MPI Examples
    - `Parallel/OpenMP` OpenMO Examples
    - `Parallel/STL` Some examples of STL parallel algorithms.
  * `Plugins`: Two examples of plugin architecture.
  * `PointInSimplex` Locate if a point is inside a 2D or 3D simplex. See the local `README.md` file.
  * `Polynomials`. A more complex example of a class that implements the generic concept of a polynomial. An example of generic programming.
  * `ReadingVector` This is an example of how you can imitate the strange way used by Eigen to read a vector from a stream by overloading the
  streaming and the comma operator. More a curiosity than something of practical use.
  * `SharedLibrary` An example of the setup of shared libraries with version control. And in `SharedLibrary/DynamicLoading` we show how to load functions
  dynamically from a shared library
  * `Templates/` A set of examples on the use of templates. Some simple, some less simple:
    - `Compare` An example of the use of policies (Bridge design pattern) as a template parameter to change the behaviour of comparison operations
    - `ExplicitInstantiation` An example of explicit instantiation of templates. A technique to save the compilation time of template instances that you use often.
    - `Fibonacci` A classic example of template metaprogramming: a recursive template. 
    - `Function` Beware of mixing template and free functions with template specializations. Here we show why you need to be careful.
    - `Interpolation` Polynomial interpolation on a set of points (x,y). Use of a policy to choose from different interpolation strategies. *Note* It differs from the code contained in the `Interp1D` folder: the latter implements just piecewise linear interpolation, but in a very generic fashion.
    - `Matrix` The initial design of a simple full matrix with row or column orderning, with the parallel implementation (with openMP) of matrix-vector product.xs
    - `Mean` An example of use of template specialization or variadic templates to create functions that compute sums and averages of values. 
    - `traits` Use of traits/policies to create an inner product able to operate seamlessly on vectors of floating points or of complex numbers. 
    - `VariadicTemplates` An illustration of variadic templates
    

## Full examples

  * `Derivatives` An example of using recursive templates to compute finite differences to approximate derivatives of any order. 
  * `ExpressionTemplates` Two implementations of Expression Templates, sufficient to understand how they work (I hope).
  * `FixedPointSolver` Utilities for solving x=F(x) via fixed point iteration. It enables the implementation of convergence accelerators, implemented as policy. I have implemented some. A complete and working example.
  * `LineSearch`. Minimization of functions using line search-type algorithms
  * `HeatExchange` A simple finite element code that solves a 1D problem. An example also of use of *GetPot*, *json file reader* and *GnuPlot*
  * `Interp1D` A generic code for piecewise linear interpolation. Implemented in a very general way to allow different specializations according to the needs.
  * `LinearAlgebra` Various tools for LinearAlgebra (and not only)
    * `CppNumericalSolvers` A submodule containing a fork of the homonymous  library [on github.](https://github.com/PatWie/CppNumericalSolvers)
    * `IML_Eigen` A porting to the [Eigen library](http://eigen.tuxfamily.org/index.php?title=Main_Page) of the algorithms for solution of sparse linear systems of equations described in [templates for the solution of linear systems](https://www.netlib.org/linalg/html_templates/Templates.html).
    * `IML_Eigen/SaddlePointSolver` A complete code for the solution of a saddle point problem arising from the discretization of a hybrid dimensional 
    Darcy problem. You find the details of the problem in [Preconditioning Techniques for the Numerical Solution of Flow in Fractured Porous Media](https://doi.org/10.1007/s10915-020-01372-0)
    * `MatrixData` Some matrices taken from [Matrix Market](https://math.nist.gov/MatrixMarket/), a repo of data for comparative studies of algorithms.
    * `MatrixMarketReadersMatlab` If you want to load the matrices into Matlab
    * `redsvd-h` A submodule containing the fork of the homonymous tool [on Github](https://github.com/ntessore/redsvd-h), which implements
    in a templated fashion the algorithms in [redsvd](https://code.google.com/archive/p/redsvd/), for solving several matrix decompositions including singular value decomposition (SVD), principal component analysis (PCA), and eigenvalue decomposition. It uses a stochastic technique to be able to extract low rank approximations of possibly very large sparse matrices at low cost. See [Finding Structure with Randomness: Probabilistic Algorithms for Constructing Approximate Matrix Decompositions](https://doi.org/10.1137/090771806)
    * `spectra` A submodule containing a fork of the homonymous tool on [GitHub](https://github.com/yixuan/spectra.git). The name is the acronim of Sparse Eigenvalue Computation Toolkit as a Redesigned ARPACK. It is a C++ library for large-scale eigenvalue problems, built on top of the Eigen linear algebra library
    * `Thomas` An implementation of the Thomas algorithm to solve tridiagonal systems of equations. It is a well known O(n) algorithm, you find the general desctiption on [Wikipedia](https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm)
    * `SparseBlockMatrix` A tool to construct block matrices where each block is a sparse Eigen matrix.
    * `Utilities` Described in the *Utilities* section.
  * `Mesh`, an example of a class for 2D meshes, with a few utilities.
  * `MyMat0/` Example of a class for Matrices with different implementations and some views.
  * `Multicity` It uses `RKFSolver` to implement a multicity epidemic model.
  * `NewtonSolver` Solving F(x)=0. It supports Newton's method, as well as quasi-Newton schemes. Indeed, it is very general. **Note** The use of a `callback`, a technique to allow getting information on the internal state of a class, when needed. A complete and working example.
  * `OneDMesh`. A class for 1D meshes. Build with a structure that mimics what you may have also in 2 or 3D meshes. It also implements an algorithm for non-uniform meshes by specifying a spacing function.
  * `QuadratureRule/` Classes and techniques for numerical integration of univariate functions. It also offers examples of dynamic loading of polymorphism, cloneable classes, dynamic loading of libraries, the decorator design pattern, and templates—a complete and working example.
  * `Regression`. An example of univariate linear regression. You can choose different polynomial models. At the moment, only least squares fitting is implemented.
  * `RK45` Class template for explicit Runge Kutta embedded schemes for scalar ordinary differential equations (Cauchy problem). Old version, now superseded by `RKFSolver`. Kept only because it provides a simpler code.
  * `RKFSolver` Class template for explicit or diagonally implicit Runge Kutta embedded schemes for scalar ordinary differential equations (Cauchy problem). Support scalar, vectorial and matricial systems (matricial system not for implicit). Highly expandable.  A complete and working example. It also contains a tool to draw the areas of absolute stability of explicit RK schemes.
    
## A DISCLAIMER ##

The code is provided as-is, without any claim of correctness or fitness for any purpose, as part of learning C++.
I have tried to find and fix all bugs I have found, but I cannot guarantee that errors aren't still present (actually, it is very likely that they are).

If you find a mistake, or if you think that an example can be bettered, **please tell me!**. I will be happy to include your corrections/suggestions, and give credits to you.

-------------------------------------------------------------------------------


