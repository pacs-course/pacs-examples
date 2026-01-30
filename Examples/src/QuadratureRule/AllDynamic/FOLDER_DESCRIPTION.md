# AllDynamic: Plugin-Based Numerical Integration

## Overview

This folder contains a complete example of **dynamic loading of quadrature rules and integrand functions** to build a plugin-style numerical integration application. The core idea is to load rules and functions from shared libraries at runtime, select them using a factory, and then run composite integration over a mesh.

The example demonstrates **runtime extensibility**, **object factories**, **dynamic linking**, and **config-driven execution** (GetPot and JSON), plus optional runtime parsing of functions with **muParser**.

---

## High-Level Architecture

1. **Factories** manage rule and integrand creation using string identifiers.
2. **Plugin libraries** register themselves into factories at load time.
3. The main executable **loads shared libraries** with `dlopen` and queries factories for the chosen rule/integrand.
4. Integration parameters are read from **GetPot** or **JSON**.
5. Integration is executed using the existing composite quadrature machinery (from the base version library).

---

## File Descriptions

### Core Application

- **main_integration.cpp**
  - Entry point of the program.
  - Loads parameter file (GetPot or JSON).
  - Dynamically loads quadrature rule libraries and integrand libraries using `dlopen`.
  - Queries factories to create the selected rule and integrand.
  - Runs composite integration and prints results.
  - Supports `--help` and `--list` to show available rules.

### Factories and Registration

- **ruleFactory.hpp / ruleFactory.cpp**
  - Specializes `GenericFactory` for quadrature rules.
  - Exposes a global factory instance: `MyFactory` (singleton-like).
  - Provides `addRuleToFactory<T>()` to register rules by name.

- **ruleProxy.hpp**
  - Alternative registration mechanism using `GenericFactory::Proxy`.
  - Shows a second approach for automatic registration using proxy objects.

### Rule Plugins (Shared Libraries)

These files are compiled as shared libraries and **register their rules** automatically when loaded:

- **AdamsRules.cpp**
  - Registers: Simpson, Trapezoidal, MidPoint, AdaptiveSimpson.

- **GaussRules.cpp**
  - Registers: Gauss-Legendre and Gauss-Lobatto rules, plus adaptive variants.

- **MontecarloRules.cpp**
  - Registers: Montecarlo rule.

Each plugin uses `__attribute__((constructor))` to ensure registration happens automatically when the library is loaded.

### Integrands and Runtime Parsing

- **udf.cpp**
  - Defines several built-in integrands.
  - Registers them into the integrand factory at load time.
  - Includes `parsedFunction` that reads the expression from `parsedFunction.txt` and evaluates via muParser.

- **udfHandler.hpp / udfHandler.cpp**
  - Defines a `FunctionFactory`-based registry for integrands.
  - Provides `addIntegrandToFactory()` with type checks.

- **muParserFunction.hpp / muParserFunction.cpp**
  - Wraps `muParser` into a callable class.
  - Reads an expression from `parsedFunction.txt`.
  - Allows runtime definition of integrands using string expressions.

### Parameters and Configuration

- **QuadParameters.hpp / QuadParameters.cpp**
  - Defines the `QuadParameters` struct with defaults.
  - Reads configuration from:
    - GetPot input files (e.g., `quadratura.getpot`)
    - JSON input files (e.g., `quadratura.json`)

- **quadratura.getpot**, **quadraturam.getpot**, **quadratura.json**
  - Example configuration files for the program.

- **parsedFunction.txt**
  - Input file containing a mathematical expression for `muParser`.

### Build System

- **Makefile**
  - Builds plugin shared libraries, the rule factory, and the main executable.
  - Depends on the base-version libraries and external packages like `muParser`.

---

## Programming Techniques Demonstrated

### 1. Dynamic Loading / Plugin Architecture
- Uses `dlopen` and runtime registration to allow new rules/integrands without recompiling the executable.
- Shared libraries register themselves into factories during load.

### 2. Factory Pattern (Object Creation by Name)
- `GenericFactory` is specialized to create rules/integrands using string IDs.
- Decouples object creation from code that uses them.

### 3. Singleton-Like Global Factory
- The rule factory is exposed as a global reference to ensure a single registry across all modules.
- Prevents subtle linker/runtime duplication issues.

### 4. Automatic Registration
- Uses `__attribute__((constructor))` in plugin files to auto-register items at load time.
- Alternative proxy-based registration shown in `ruleProxy.hpp`.

### 5. Runtime Function Parsing
- `muParser` allows integrands to be written in text files, parsed at runtime.
- Provides flexibility for user-defined functions without recompilation.

### 6. Configuration-Driven Execution
- `GetPot` and JSON input allow all integration options to be specified externally.
- Supports batch runs and easy experimentation.

### 7. Type Safety and Constraints
- `static_assert` ensures only callable objects are registered as integrands.
- Strong typing in factories prevents invalid object creation.

---

## Learning Objectives

This example demonstrates how to:

- Build a plugin system in C++ using shared libraries
- Implement object factories with runtime registration
- Combine polymorphism and templates for extensibility
- Parse user-defined functions at runtime
- Use GetPot and JSON for configuration
- Build flexible scientific software with strong modularity

---

## Notes

- The code depends on the **baseVersion** libraries for quadrature rule implementations and numerical integration logic.
- `muParser` must be installed and correctly linked for parsed functions.
- The factory library **must** be shared, otherwise rule registration can fail due to multiple instances of the factory.
