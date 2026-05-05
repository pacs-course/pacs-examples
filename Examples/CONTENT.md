# Content of `Examples/src`

This file gives a short overview of the examples contained in `src/`. Each
example directory also provides its own `README.md` with more details.

The collection is broad, but most folders fall into one of these groups:

- reusable utilities used by other examples;
- language and library examples focused on modern C++;
- numerical and scientific-computing examples;
- parallel-programming and interoperability examples.

## Utilities and Support Code

- `Utilities`: general-purpose utilities used by many other examples. Install
  this first with `make` and `make install`.
- `LinearAlgebraUtil`: additional utilities for linear algebra and numerical
  work. Install with `make install`.
- `GenericFactory`: reusable implementation of a generic object factory.
- `LoadLibraries`: helper utility for loading shared libraries at run time.
- `Matrix`: simple dense matrix class used by some examples.
- `OneDMesh`: one-dimensional mesh class and generators.
- `adtTree`: adaptive tree data structure.
- `bcHandler`: utilities for handling boundary conditions.
- `muParserInterface`: examples of how to interface with muParser and
  muParserX.

## Core C++ Language and Library Examples

- `Aggregates`: aggregate types and aggregate initialization.
- `Arrays`: examples based on `std::array`.
- `Bindings`: reference binding rules, forwarding, and move-related behavior.
- `Concepts`: user-defined concepts and constrained templates.
- `Constexpr`: compile-time computation with `constexpr`.
- `CRTP`: Curiously Recurring Template Pattern.
- `CompositionWithVariadicTemplates`: composition techniques based on variadic
  templates.
- `ForwardDeclaration`: when and why forward declarations are needed.
- `Functors`: callable objects and `std::function`.
- `HeapView`: simple heap-based view utilities.
- `Horner`: Horner's rule for polynomial evaluation.
- `IntegerOverflow`: examples showing the pitfalls of integer overflow.
- `LambdaExpr`: lambda expressions and their typical use.
- `Linkage`: external linkage, internal linkage, and translation-unit scope.
- `MoveSemantic`: move semantics in a more complete setting.
- `MoveSemantic_simple`: compact illustration of move construction and move
  assignment.
- `Numeric_Limits`: examples based on `std::numeric_limits`.
- `Odr`: the One Definition Rule and what may go wrong when it is violated.
- `OrderingAndSpaceShip`: C++20 comparison categories and the spaceship
  operator.
- `Polygon`: basic runtime polymorphism on a polygon hierarchy.
- `Preprocessor`: preprocessing, macro definitions, and generated source.
- `Rational`: operator overloading through a rational-number class.
- `ReadingVectors`: stream extraction utilities inspired by Eigen's syntax.
- `ReferenceWrapper`: storing references indirectly via `std::reference_wrapper`.
- `Scope`: examples on scoping rules.
- `SimpleProgram`: minimal introductory example.
- `SmartPointers`: ownership and resource management with smart pointers.
- `StaticMembers`: examples involving static class members.
- `StringLiteralsAndViews`: string literals and string/view-related features.
- `Vector`: examples related to `std::vector`.

## Standard Library Focus

- `STL/cont`: container examples.
- `STL/FileSystem`: traversal and inspection of file systems.
- `STL/InitializerList`: use of initializer lists.
- `STL/Map`: associative containers based on maps.
- `STL/Niebloids`: standard-library function objects and customization points.
- `STL/Optional`: use of `std::optional`.
- `STL/RangesAndViews`: ranges, views, and constrained algorithms.
- `STL/Reduce`: `reduce` and related parallel-friendly reductions.
- `STL/Set`, `STL/SetEdge`: set-related examples.
- `STL/Sort`, `STL/SortAndPermute`: sorting and permutation patterns.
- `STL/Span`: use of `std::span`.
- `STL/UnorderedMap`: hash-based associative containers.
- `STL/Variant`: use of `std::variant`.
- `STL/mdspan`: examples based on `mdspan`.
- `STL/tuple`: tuple-related utilities and idioms.

## Metaprogramming, Templates, and Design Techniques

- `MetaProgramming`: template metaprogramming examples such as `decltype`,
  `declval`, traits, tuple traversal, and compile-time recursion.
- `Templates/Compare`: policy-based customization of comparisons.
- `Templates/ExplicitInstantiation`: explicit template instantiation.
- `Templates/Fibonacci`: classic recursive template metaprogramming example.
- `Templates/Functions`: interactions among function templates, overloads, and
  specialization.
- `Templates/Interpolation`: interpolation strategies selected through
  templates and policies.
- `Templates/Mean`: specialization and variadic templates for means and sums.
- `Templates/VariadicTemplates`: examples of variadic templates.
- `Templates/traits`: traits and policies for generic inner products.
- `DesignPatterns`: examples of common design patterns.
- `ExpressionTemplates`: expression-template techniques.
- `Plugins`: plugin architectures based on dynamic loading and runtime
  registration.
- `SharedLibrary`: building shared libraries and loading them dynamically.

## Floating Point, Randomness, and Performance

- `FloatingPoint`: floating-point comparison, cancellation, quadratic roots,
  and floating-point exceptions.
- `RandomDistibutions`: random engines, probability distributions, and sample
  statistics.
- `OptimizationAndProfiling`: optimization, profiling, aliasing, leaks, and
  related low-level topics.
- `Streams`: formatted I/O, file I/O, serialization, binary I/O, and stream
  redirection.

## Parallel Programming

- `Parallel/MPI`: distributed-memory examples with MPI.
- `Parallel/OpenMP`: shared-memory examples with OpenMP.
- `Parallel/ParallelSTL`: C++ standard-library parallel algorithms.
- `Parallel/Utilities`: helper code used by some parallel examples.

## Numerical and Scientific-Computing Examples

- `Derivatives`: finite-difference approximations of derivatives.
- `FixedPointSolver`: fixed-point solvers and acceleration strategies.
- `HeatExchange`: one-dimensional finite-element example.
- `InPolygon`: point-in-polygon tests.
- `Interp1D`: generic piecewise-linear interpolation.
- `LineSearch`: line-search methods for minimization.
- `LinearAlgebra`: linear-algebra-related packages and example codes, including
  `IML_Eigen`, `SparseBlockMatrix`, `Thomas`, `spectra`, and other material.
- `Mesh`: simple two-dimensional mesh structures and utilities.
- `MultiCity`: multicity epidemic model built on top of time-integration
  utilities.
- `MyMat0`: matrix-class experiments, including views and traits.
- `NewtonSolver`: Newton and quasi-Newton methods for nonlinear equations.
- `NonLinSys`: examples on nonlinear systems.
- `PointInSimplex`: point location in 2D and 3D simplices.
- `Polynomials`: generic polynomial class and related operations.
- `QuadratureRule`: numerical quadrature, adaptive rules, decorators, and
  dynamic loading.
- `Regression`: univariate polynomial regression.
- `Richardson`: Richardson extrapolation.
- `RK45`: older Runge-Kutta example kept as a simpler reference.
- `RKFSolver`: explicit and diagonally implicit embedded Runge-Kutta solvers.
- `SegmentIntersect`: segment-intersection algorithms.

## Python Interoperability

- `pybind11`: examples showing how to expose C++ code to Python with
  `pybind11`, including bindings for numerical and utility code.

## Disclaimer

The code is provided as teaching material. It is meant to illustrate ideas and
techniques, not to serve as production-ready software. While many examples are
fully usable, some are intentionally small, simplified, or exploratory.
