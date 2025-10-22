# NonLynSys Folder Documentation

## Overview
The `NonLynSys` folder contains utilities for managing nonlinear systems of equations. It provides a flexible framework for defining systems of scalar functions, evaluating them, and extending their functionality through traits and factories.

## Key Components

### 1. `NonLinSys` Class
The `NonLinSys` class represents a nonlinear system $\mathbb{R}^n \to \mathbb{R}^m$. It stores a collection of m scalar functions $\mathbb{R}^n \to \mathbb{R}$ and provides methods to evaluate the system, add functions, and update existing ones.

#### Template Parameters:
- `R`: The scalar type (default: `double`).
- `Traits`: A traits class defining types used in the system (default: `VectorTraits`).

#### Key Methods:
- `addToSystem`: Adds a scalar function to the system.
- `getFunction`: Retrieves a function by index.
- `updateFunction`: Updates a function at a specific index.
- `operator()`: Evaluates the system for a given input.

#### Example:
```cpp
apsc::NonLinSys<double, ScalarTraits> system;
system.addToSystem([](double const &x) { return x * x; });
std::cout << "Function 0(2.0)=" << system.getFunction(0)(2.0) << std::endl;
```

### 2. `NonLinSysTraits` Namespace
This namespace defines traits for customizing the behavior of `NonLinSys`. Traits specify types for arguments, results, and function containers.

#### Available Traits:
- `VectorTraits`: Uses `std::vector` for arguments and results.
- `EigenVectorTraits`: Uses `Eigen::VectorXd` for arguments and results.
- `ScalarTraits`: Uses scalars as arguments and vectors as results.

#### Example:
```cpp
using EigenTraits = apsc::NonLinSysTraits::EigenVectorTraits<double>;
apsc::NonLinSys<double, EigenTraits> system;
```

### 3. `FunctionFactory` Class
The `FunctionFactory` class extends `NonLinSys` to manage functions with unique string identifiers. It ensures that each function has a unique name and provides methods to retrieve functions by their identifiers.

#### Key Methods:
- `addToFactory`: Adds a function with a unique identifier.
- `getFunction`: Retrieves a function by its identifier.

#### Example:
```cpp
apsc::FunctionFactory<double, ScalarTraits> factory;
factory.addToFactory("sin", [](double const &x) { return std::sin(x); });
std::cout << "sin(3.14)=" << factory.getFunction("sin")(3.14) << std::endl;
```

## Usage Examples
The `main_nls.cpp` file demonstrates the usage of `NonLinSys` and `FunctionFactory`:

1. **Creating a System**:
   ```cpp
   apsc::NonLinSys<double, ScalarTraits> system;
   system.addToSystem([](double const &x) { return x * x; });
   ```

2. **Using Eigen Vectors**:
   ```cpp
   apsc::NonLinSys<double, apsc::NonLinSysTraits::EigenVectorTraits> system2;
   system2.addToSystem([](Eigen::VectorXd const &x) { return x.norm(); });
   ```

3. **Function Factory**:
   ```cpp
   apsc::FunctionFactory<double, ScalarTraits> factory;
   factory.addToFactory("cos", [](double const &x) { return std::cos(x); });
   ```

## Dependencies
- **Eigen**: For linear algebra operations.
- **C++20 Concepts**: Used for type constraints.
- **OpenMP**: Optional, for parallel execution.

## Build Instructions
1. Ensure Eigen is installed.
2. Compile with a C++20-compatible compiler:
   ```bash
   g++ -std=c++20 -fopenmp -I/path/to/eigen main_nls.cpp -o main_nls
   ```
3. Run the executable:
   ```bash
   ./main_nls
   ```

## Conclusion
The `NonLynSys` folder provides a robust framework for nonlinear systems, leveraging modern C++ features for flexibility and performance. The examples in `main_nls.cpp` illustrate its versatility and ease of use.

## Interaction with other codes
`NonLinSys` may be used in conjunction with the code in `NewtonSolver` for the solution of non-linear system of equations.