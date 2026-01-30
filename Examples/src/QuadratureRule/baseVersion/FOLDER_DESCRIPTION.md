# BaseVersion: Numerical Integration Library

## Overview

This directory contains a comprehensive C++ library for numerical integration using various quadrature rules. The implementation demonstrates advanced C++ concepts including polymorphism, the Strategy and Decorator design patterns, and modern C++17/20 features.

## Architecture

The library is structured around a polymorphic class hierarchy with `QuadratureRuleBase` as the abstract base class, allowing different integration methods to be used interchangeably through a common interface.

### Core Design Patterns

1. **Strategy Pattern**: The `Quadrature` class uses quadrature rules as policies
2. **Decorator Pattern**: Adaptive rules and error estimators enhance basic quadrature rules
3. **Virtual Constructor (Clone) Idiom**: All rules implement `clone()` for deep copying

---

## File Descriptions

### Header-Only Base Classes

#### `QuadratureRuleTraits.hpp`
- Defines fundamental types used throughout the library
- `FunPoint`: Type alias for `std::function<double(double const &)>` representing integrand functions
- Uses `CloningUtilities.hpp` for the PointerWrapper pattern

#### `QuadratureRuleBase.hpp`
- **Abstract base class** for all quadrature rules
- Pure virtual methods:
  - `clone()`: Virtual constructor for deep copying
  - `apply(f, a, b)`: Apply the rule to integrate function `f` from `a` to `b`
  - `name()`: Return string identifier for the rule
- Optional virtual methods for adaptive rules: `setTargetError()`, `setMaxIter()`
- Defines `QuadratureRuleHandler` using `apsc::PointerWrapper`

#### `StandardQuadratureRule.hpp`
- **Template base class** for standard quadrature rules on `[-1,1]`
- Template parameter `NKNOTS`: number of quadrature nodes
- Stores nodes and weights as `std::array<double, NKNOTS>`
- Implements the `apply()` method with automatic scaling from `[-1,1]` to `[a,b]`
- All derived classes are final and implement `clone()`

#### `QuadratureRulePlusError.hpp`
- **Decorator template class** that adds error estimation to any standard rule
- Uses Richardson extrapolation comparing integral over `h` vs `h/2`
- Contains nested `Error` struct to store error estimates
- **Design note**: Uses `inline static` member for the underlying rule (shared across all instances)
- Mutable `ExtractError` member allows error access from const methods

#### `QuadratureRuleAdaptive.hpp`
- **Decorator template class** implementing adaptive quadrature
- Template parameter: any standard quadrature rule class
- Uses a queue-based algorithm to subdivide intervals where error exceeds target
- Parameters:
  - `targetError`: Desired accuracy (default: 1.e-6)
  - `maxIter`: Maximum number of subdivisions (default: 100)
- Delegates to `QuadratureRulePlusError<SQR>` for error estimation

---

### Concrete Quadrature Rules

#### `Adams_rule.hpp` / `Adams_rule.cpp`
Implements Newton-Cotes formulas:
- **`Simpson`**: 3-point rule, order 4 (cubic accuracy)
- **`MidPoint`**: 1-point rule, order 2 
- **`Trapezoidal`**: 2-point rule, order 2

#### `Gauss_rule.hpp`
Implements Gaussian quadrature rules (header-only):
- **`GaussLegendre2p`**: 2-point rule, order 4
- **`GaussLegendre3p`**: 3-point rule, order 6
- **`GaussLegendre4p`**: 4-point rule, order 8
- **`GaussLobatto4p`**: 4-point rule with endpoints, order 6
- **`GaussLobatto5p`**: 5-point rule with endpoints, order 8

**Note**: Uses `inline static const` for computed constants (C++17 feature)

#### `montecarlo.hpp` / `montecarlo.cpp`
Implements Monte Carlo integration:
- Uses `std::knuth_b` random engine with `std::random_device` seed
- Computes running mean and standard deviation
- Configurable error tolerance and maximum iterations
- **C++17 feature**: `inline static std::random_device rd;`
- Tracks cumulative error and iteration count

---

### Composite Integration

#### `numerical_integration.hpp` / `numerical_integration.cpp`
Contains the **`Quadrature`** class implementing composite integration:
- Aggregates a `Mesh1D` (1D mesh from geometry library)
- Composes with a `QuadratureRuleBase` via `QuadratureRuleHandler`
- Applies the rule to each mesh interval and sums results
- **Features**:
  - Perfect forwarding constructors (universal references)
  - Support for clone-based construction from base class references
  - Rule of five with default implementations
  - Three implementation variants (via preprocessor):
    - Standard loop
    - OpenMP parallel reduction (`_OPENMP`)
    - Parallel STL with `std::transform_reduce` (`PARALLELCPP`)

---

### Utilities

#### `helperfunction.hpp` / `helperfunction.cpp`
Helper functions for parameter management:
- `readParameters()`: Reads integration parameters from command line (uses GetPot)
  - Supports reading integration bounds, number of intervals, target error
  - Overloaded version for dynamic library loading
- `printout()`: Formatted output of integration results

#### `integrands.hpp` / `integrands.cpp`
Test integrand functions:
- `fsincos()`: Test function (likely `sin(x) + cos(x)` or similar)
- `exact()`: Analytical solution for verification
- Defines mathematical constant `pi`

---

### Build System

#### `Makefile`
Sophisticated build system producing three libraries:
- **`libquadrules`**: Basic quadrature rule implementations
- **`libquadrature`**: Composite quadrature (depends on `libMesh1D`)
- **`libintegrands`**: Test integrand functions

**Build options**:
- Static or dynamic linking (`LIBTYPE`)
- Debug or release mode
- OpenMP support
- Parallel C++ (`PARALLELCPP`)

**Dependencies**:
- `libMesh1D` (1D mesh library)
- GetPot (parameter parsing)

---

### Application

#### `main_integration.cpp`
Comprehensive test program demonstrating:
- Standard rules: Simpson, MidPoint, Trapezoidal, Gauss-Legendre
- Monte Carlo integration with error tracking
- Adaptive integration with Simpson and Gauss-Lobatto rules
- Comparison with exact solution
- Parameter reading from command line

**Usage**:
```bash
./main_integration [options]
```

---

## Key C++ Features Demonstrated

### Modern C++ (C++17/20)
- `inline static` data members (C++17)
- `constexpr` constructors and methods
- `std::unique_ptr` for ownership
- Universal references and perfect forwarding
- Structured bindings (potential for C++20 upgrade)
- `std::optional` usage (implicit)

### Design Patterns
- **Strategy Pattern**: Quadrature rules as interchangeable policies
- **Decorator Pattern**: Adaptive and error-estimation wrappers
- **Virtual Constructor**: Clone idiom for polymorphic copying
- **Template Method**: Base class defines algorithm skeleton

### Advanced Techniques
- CRTP (Curiously Recurring Template Pattern) potential
- Type erasure via polymorphism
- Policy-based design with templates
- Mutable members for const-correctness
- Smart pointer management

### Parallel Computing
- OpenMP parallel reductions
- C++17 parallel algorithms (`std::execution::par_unseq`)
- Thread-safe random number generation considerations

---

## Known Issues and Improvements

### Critical Issues
1. **`Gauss_rule.hpp:95`**: Typo in `GaussLobatto5p` weights - `1.10` should be `1./10`
2. **`QuadratureRulePlusError.hpp:86`**: `inline static` member shared across all template instances may cause issues

### Design Concerns
1. Monte Carlo error estimation in first iteration (uninitialized `S`)
2. No error checking for `dynamic_cast` in `main_integration.cpp`
3. Shared static rule in `QuadratureRulePlusError` limits flexibility

### C++20 Modernization Opportunities
1. Add `[[nodiscard]]` to query methods
2. Use C++20 concepts instead of SFINAE
3. Replace iterator loops with `std::ranges` and `std::views::zip`
4. Add `constexpr` to more methods
5. Use designated initializers for clarity

---

## Building and Running

```bash
# Clean build
make distclean

# Build all libraries (release mode)
make alllibs DEBUG=no

# Install libraries to PACS_LIB_DIR
make install

# Build and run test program
make exec
```

---

## Dependencies

- **C++ Standard**: C++17 minimum (C++20 recommended)
- **External Libraries**:
  - `libMesh1D`: 1D mesh generation and management
  - GetPot: Command-line and configuration file parsing
  - `CloningUtilities.hpp`: Pointer wrapper utilities
- **Optional**:
  - OpenMP (for parallel loops)
  - TBB or other parallel execution backend (for C++17 parallel algorithms)

---

## Learning Objectives

This example demonstrates:
1. **Polymorphism**: Abstract base classes with virtual methods
2. **Design Patterns**: Strategy, Decorator, Virtual Constructor
3. **Template Metaprogramming**: Template classes with non-type parameters
4. **Modern C++**: Smart pointers, perfect forwarding, inline static members
5. **Code Organization**: Header-only vs. compiled libraries
6. **Parallel Programming**: OpenMP and parallel STL
7. **Software Engineering**: Modular design, separation of concerns, reusability

---

## References

- Decorator Pattern: [Refactoring Guru](https://refactoring.guru/design-patterns/decorator)
- Gaussian Quadrature: Numerical analysis textbooks
- C++17 Features: [cppreference.com](https://en.cppreference.com/)
