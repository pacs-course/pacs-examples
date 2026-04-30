# Base Version Of The Quadrature Library

This directory contains the core code used by the numerical integration
examples. It defines:

- the hierarchy of quadrature rules
- the `CompositeQuadrature` class, which applies a rule on each interval of a
  mesh
- decorators for error estimation and adaptive integration
- a small executable, `main_integration`, used as a test and demonstration

## Main Components

- `QuadratureRuleTraits.hpp`
  Basic types used throughout the quadrature code.

- `QuadratureRuleBase.hpp`
  Abstract base class for all quadrature rules.

- `StandardQuadratureRule.hpp`
  Base class for standard quadrature rules defined on a reference interval.

- `Adams_rule.hpp` / `Adams_rule.cpp`
  Newton-Cotes rules such as MidPoint, Trapezoidal, and Simpson.

- `Gauss_rule.hpp`
  Gaussian and Gauss-Lobatto quadrature rules.

- `QuadratureRulePlusError.hpp`
  Decorator adding an error estimate to a standard rule.

- `QuadratureRuleAdaptive.hpp`
  Decorator implementing adaptive integration.

- `montecarlo.hpp` / `montecarlo.cpp`
  Monte Carlo integration rule.

- `numerical_integration.hpp` / `numerical_integration.cpp`
  Definition and implementation of `CompositeQuadrature`.

- `helperfunction.hpp` / `helperfunction.cpp`
  Utility functions to read parameters and print formatted results.

- `integrands.hpp` / `integrands.cpp`
  Sample integrands and exact reference values.

- `main_integration.cpp`
  Example driver exercising the library.

## Produced Libraries

The Makefile builds three libraries:

- `libquadrules`
  Basic quadrature rules and Monte Carlo rules.

- `libquadrature`
  Composite quadrature support, including `CompositeQuadrature`.

- `libintegrands`
  Sample integrand functions used by the test program.

## Dependencies

Before building this directory, make sure the required dependencies are
available.

### Required project dependency

`libquadrature` depends on `libMesh1D`, so `Examples/src/OneDMesh` must be
built and installed first.

Recommended order:

```bash
cd Examples/src/OneDMesh
make alllibs DEBUG=no
make install
```

### External dependency

The code also uses `GetPot` for parameter handling. The repository-wide build
configuration must already know where `GetPot` is installed.

## Compilation Process

This folder uses the common PACS make infrastructure. The most useful targets
are:

```bash
make
make static
make dynamic
make alllibs
make exec
make clean
make distclean
make install
```

### Meaning of the targets

- `make static`
  Builds the three static libraries:
  `libquadrules.a`, `libquadrature.a`, and `libintegrands.a`.

- `make dynamic`
  Builds the three shared libraries:
  `libquadrules.so`, `libquadrature.so`, and `libintegrands.so`.

- `make alllibs`
  Builds both the static and dynamic versions of all three libraries.

- `make exec`
  Compiles the example executable `main_integration`.
  This target assumes the required libraries are already available either in
  the current directory or in the PACS library directory, depending on the
  build mode.

- `make clean`
  Removes object files and executables but keeps built libraries.

- `make distclean`
  Removes executables, object files, dependency files, and generated
  libraries.

- `make install`
  Builds both static and dynamic libraries first, then installs headers and
  libraries into the PACS include/lib directories.

### Build options

- `DEBUG=no`
  Enables release-style compilation and defines `NDEBUG`.

- `DEBUG=yes`
  Keeps debug information and uses the local libraries when linking the test
  executable.

- `LIBTYPE=STATIC` or `LIBTYPE=DYNAMIC`
  Affects which kind of library is used in the recursive build targets.

## Recommended Build Workflows

### 1. Build only the libraries

```bash
make distclean
make alllibs DEBUG=no
```

This is the standard choice if you want to use the quadrature libraries from
other examples.

### 2. Build the example executable for local testing

```bash
make distclean
make alllibs
make exec
```

This produces the libraries locally and then builds `main_integration`.

### 3. Install the libraries and headers

```bash
make distclean
make install DEBUG=no
```

Since `install` already depends on `alllibs`, this is enough to:

1. build the static and shared libraries
2. copy public headers to `$(PACS_INC_DIR)`
3. move the generated libraries to `$(PACS_LIB_DIR)`

## What `make install` Does

The installation step copies or moves the following artefacts:

- headers from the rule and composite quadrature parts into
  `$(PACS_INC_DIR)`
- `libquadrules.a`, `libquadrules.so`
- `libquadrature.a`, `libquadrature.so`

into `$(PACS_LIB_DIR)`

Note that the install rule moves the generated libraries out of the local
directory, so after installation the local folder no longer contains the built
libraries.

## Running The Example

After building the executable:

```bash
./main_integration
```

If you compile in release mode and rely on installed shared libraries, you may
need:

```bash
export LD_LIBRARY_PATH=$(PACS_LIB_DIR):.
```

This is consistent with the note already printed by the Makefile help output.

## What You Learn Here

- use of polymorphism through a common quadrature-rule base class
- strategy-based design through `CompositeQuadrature`
- decorator-based extensions for adaptive integration and error estimation
- separation between rules, mesh-based integration, and sample test functions
