# Dynamic Quadrature Plugins

This directory contains the dynamic-loading version of the quadrature example.
Instead of selecting rules and integrands only at compile time, the executable
loads shared libraries at runtime and discovers available rules and functions
through factories.

The composite integration step is performed by the `CompositeQuadrature` class
from `QuadratureRule/baseVersion`, while this folder adds:

- runtime loading of quadrature-rule plugins
- runtime loading of integrand libraries
- a shared factory used by all loaded modules
- optional integrands parsed at runtime with `muParser`

Compared with the `baseVersion` example, this directory focuses less on the
quadrature algorithm itself and more on the plugin architecture around it:
shared factories, dynamic registration, runtime discovery, and late binding of
both rules and user-defined functions.

## Main Components

- `main_integration.cpp`
  Main executable. It reads the input file, loads plugin libraries with
  `dlopen`, queries the factories, and runs the integration.

- `ruleFactory.hpp` / `ruleFactory.cpp`
  Shared factory for quadrature rules.

- `ruleProxy.hpp`
  Alternative registration mechanism for factory entries.

- `udfHandler.hpp` / `udfHandler.cpp`
  Shared factory for integrand callables.

- `udf.cpp`
  Integrand library, including the bridge to runtime-parsed functions.

- `muParserFunction.hpp` / `muParserFunction.cpp`
  Utility class wrapping `muParser`.

- `AdamsRules.cpp`, `GaussRules.cpp`, `MontecarloRules.cpp`
  Shared plugin libraries registering different rule families.

- `QuadParameters.hpp` / `QuadParameters.cpp`
  Utilities to read parameters from `GetPot` or JSON input files.

- `quadratura.getpot`, `quadraturam.getpot`, `quadratura.json`
  Example input files.

- `parsedFunction.txt`
  Text file containing the expression used by the parsed integrand example.

## Produced Shared Libraries

This folder builds only dynamic libraries:

- `librulesFactory.so`
  Shared quadrature-rule factory.

- `libudf.so`
  Shared integrand library.

- `libAdamsRules.so`
  Plugin with Newton-Cotes rules.

- `libGaussRules.so`
  Plugin with Gauss and Gauss-Lobatto rules.

- `libMontecarloRules.so`
  Plugin with Monte Carlo rules.

The executable is:

- `main_integration`

## Dependencies

This example depends on both project libraries and external packages.

### Project dependencies

Before building `AllDynamic`, install the libraries from:

1. `Examples/src/OneDMesh`
2. `Examples/src/QuadratureRule/baseVersion`
3. `Examples/src/LoadLibraries`

Recommended order:

```bash
cd Examples/src/OneDMesh
make alllibs DEBUG=no
make install

cd ../LoadLibraries
make alllibs
make install

cd ../QuadratureRule/baseVersion
make alllibs DEBUG=no
make install


cd ../AllDynamic
```

`AllDynamic` uses:

- `libMesh1D`
- `libquadrules`
- `libquadrature`

from those installed locations.

### External dependencies

The folder also relies on:

- `GetPot`
- `muParser`
- the POSIX dynamic loading library `dl`

Before building this example, `muParser` must be installed and visible to the
global build configuration.

## Compilation Process

This folder is intentionally built around shared libraries only. The Makefile
does not provide static library targets and does not provide an `install`
target.

The useful commands are:

```bash
make
make dynamic
make alllib
make exec
make clean
make distclean
```

### Meaning of the targets

- `make dynamic`
  Builds all shared libraries used by the plugin system.

- `make alllib`
  Alias for `make dynamic`.

- `make exec`
  Builds the shared libraries first and then links `main_integration`.

- `make clean`
  Removes object files and the executable but keeps generated `.so` files.

- `make distclean`
  Removes objects, executable, shared libraries, and dependency files.

### Build options

- `DEBUG=no`
  Enables optimized compilation and keeps the runtime rpath pointing to
  `$(PACS_LIB_DIR)`.

- `DEBUG=yes`
  Keeps debug information while still using the same installed-library search
  path.

## Recommended Build Workflow

From a clean tree:

```bash
make distclean
make exec DEBUG=no
```

This is the most practical command because `exec` already depends on `alllib`,
which in turn builds all required plugin libraries.

If you only want to rebuild the plugin libraries and not the executable:

```bash
make distclean
make dynamic DEBUG=no
```

## Installation Note

Unlike `QuadratureRule/baseVersion`, this folder does **not** define a
`make install` target.

That is consistent with the role of the folder:

- it is primarily an executable example
- its plugin shared libraries are meant to be loaded from the local directory
  during testing
- its main reusable dependencies are already installed from `OneDMesh` and
  `baseVersion`

If you want to reuse the generated plugins elsewhere, you currently need to
copy the `.so` files manually or extend the Makefile with an installation rule.

## Running The Example

After building:

```bash
./main_integration
```

or with an explicit input file:

```bash
./main_integration InputFile=quadratura.getpot
./main_integration InputFile=quadratura.json
```

Useful command-line options:

- `-h` or `--help`
  Show usage information.

- `-l` or `--list`
  List the rules currently registered after loading the selected plugin
  libraries.

Since the executable loads shared libraries at runtime, make sure the dynamic
loader can find both the installed PACS libraries and the local plugin
libraries. In practice this usually means:

```bash
export LD_LIBRARY_PATH=$(PACS_LIB_DIR):.
```

## Typical Runtime Flow

At execution time, the program works roughly as follows:

1. read the input file and determine which plugins and integrands are needed
2. load the shared libraries with `dlopen`
3. let each loaded module register its objects into the shared factories
4. query the factories to create the requested quadrature rule and integrand
5. run the composite quadrature algorithm and print the result

This sequence is the key idea of the example: the executable knows only the
common interfaces and the factories, while the concrete implementations remain
in separately compiled shared libraries.

## How The Build Is Structured

The shared-library dependency graph is roughly:

- `librulesFactory.so`
  Must be shared and common to all plugin modules so that every loaded object
  sees the same factory instance.

- `libAdamsRules.so`, `libGaussRules.so`, `libMontecarloRules.so`
  Register quadrature rules into `librulesFactory.so` when loaded.

- `libudf.so`
  Registers integrands into the integrand factory and links to `muParser`.

- `main_integration`
  Links against `libquadrature`, `libMesh1D`, `librulesFactory.so`, and `dl`,
  then loads the actual rule and integrand plugins at runtime.

This shared-factory arrangement is essential. If the factory lived in ordinary
object code instead of a shared library, different modules could end up seeing
different registries.

## What You Learn Here

- how to implement a plugin architecture with `dlopen`
- how to register objects into shared factories at load time
- how to combine runtime extensibility with the `CompositeQuadrature` solver
- how to integrate compiled integrands and runtime-parsed expressions in one
  framework
