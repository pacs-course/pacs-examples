# A class for nonlinear systems of functions

This example shows how to build a flexible C++ class that stores a set of scalar functions and can be used:

1. as a full system of equations;
2. as a collection of individually callable equations.

The key idea is to represent a system \(R^n \to R^m\) as a container of functions \(R^n \to R\).  
Each equation is stored as a `std::function`, so lambdas and other callable objects are supported.

## Traits-based design

The argument and return types are configured through traits (`NonLinSysTraits.hpp`), so you can switch between:

- scalar arguments;
- `std::vector` arguments;
- Eigen vector arguments.

This makes the same `NonLinSys` class reusable in different numerical settings.

## Calling the system

You can evaluate a single equation:

```cpp
apsc::NonLinSys<> sys;
std::vector<double> x;
double y = sys.getFunction(i)(x);
```

Or evaluate the whole system at once:

```cpp
apsc::NonLinSys<> sys;
std::vector<double> x;
auto y = sys(x);
```

To use scalar input and vector output, select the scalar traits:

```cpp
apsc::NonLinSys<double, apsc::NonLinSysTraits::ScalarTraits> system;
```

The folder also includes a `FunctionFactory` built on top of `NonLinSys`, where each function is registered with a unique string identifier.

## Parallel implementation

Evaluating all equations is naturally embarrassingly parallel.

- `make parallel`: enables C++ parallel algorithms (`std::execution`, requires TBB).
- `make parallel_omp`: enables OpenMP-based parallelization.
- `make`: builds the serial version.

Use `make -B` to force recompilation.

# What does this example show?

- Use of `std::for_each` in a parallel context.
- Practical usage of `std::function` with lambdas.
- A traits-based design with template-template parameters.
- A simple function factory pattern.
- Two parallel backends (standard parallel algorithms and OpenMP).
