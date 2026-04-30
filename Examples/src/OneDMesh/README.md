# One-Dimensional Mesh Utilities

This directory provides a small library for building and storing one-dimensional
meshes on an interval. The main class is `Geometry::Mesh1D`, while the actual
node generation is delegated to mesh-generator policies derived from
`Geometry::OneDMeshGenerator`.

Two mesh-generation strategies are provided:

- `Uniform`
  Generates equally spaced nodes on a `Domain1D`.

- `VariableSize`
  Generates a mesh with prescribed local spacing by integrating a spacing
  function with the Runge-Kutta-Fehlberg solver from `RKFSolver`.

## Main Files

- `domain.hpp` / `domain.cpp`
  Defines `Geometry::Domain1D`, a simple interval class.

- `meshGenerators.hpp` / `meshGenerators.cpp`
  Defines the generator interface and the concrete `Uniform` and
  `VariableSize` policies.

- `mesh.hpp` / `mesh.cpp`
  Defines `Geometry::Mesh1D`, which stores mesh nodes and exposes utilities
  such as `numNodes()`, `hmin()`, and `hmax()`.

- `main_testGenerators.cpp`
  Small driver that builds a uniform and a variable-size mesh, writes the
  nodes to `uniform.dat` and `variable.dat`, and prints basic statistics.

- `run_testGenerator.sh`
  Helper script to run the test and visualize the results with `gnuplot`.

## Design Notes

The code is a compact example of the strategy pattern:

- `Mesh1D` stores the resulting mesh
- `OneDMeshGenerator` defines the generation interface
- concrete generators decide how nodes are computed

The variable-size generator is more interesting numerically. Given a positive
spacing law `h(x)`, it integrates `1 / h(x)` and then interpolates the inverse
map to position the internal nodes.

## Dependencies

`VariableSize` depends on the RKF ODE solver, and the RKF solver in turn uses
the Newton solver utilities for implicit methods. Because of that, the practical
dependency chain is:

1. `Examples/src/NewtonSolver`
2. `Examples/src/RKFSolver`
3. `Examples/src/OneDMesh`

Before building this folder for full use, install the prerequisites.

### Recommended prerequisite build order

From the repository root, or by entering each directory manually:

```bash
cd Examples/src/NewtonSolver
make alllibs DEBUG=no
make install

cd ../RKFSolver
make alllibs DEBUG=no
make install

cd ../OneDMesh
```

## Building

This folder uses the common PACS make infrastructure. Typical targets are:

```bash
make
make dynamic
make static
make exec
make alllibs
make clean
make distclean
```

Meaning of the most relevant targets:

- `make dynamic`
  Builds `libMesh1D.so`.

- `make static`
  Builds `libMesh1D.a`.

- `make exec`
  Builds the example executable `main_testGenerators`, assuming the required
  library configuration is already available.

- `make alllibs`
  Builds both `libMesh1D.a` and `libMesh1D.so`.

- `make install`
  Installs headers and both libraries into the PACS include/lib directories.

Build options:

- `DEBUG=yes` keeps debug information.
- `DEBUG=no` enables `-DNDEBUG` and optimized compilation.
- `LIBTYPE=DYNAMIC` or `LIBTYPE=STATIC` selects the intended library mode when
  relevant.

## Recommended Build Workflow

Because of the current Makefile structure, the most reliable workflow for a
clean tree is:

```bash
make distclean
make dynamic DEBUG=no
make exec LIBTYPE=DYNAMIC DEBUG=no
```

This sequence:

1. builds the shared library
2. links the executable against that shared library
3. preserves the transitive dependency on `libNewton`

You can then run:

```bash
./main_testGenerators
```

## Installation

To install the library for use by other examples:

```bash
make distclean
make alllibs DEBUG=no
make install
```

The install target copies:

- the library headers (`*.hpp`) to `$(PACS_INC_DIR)`
- `libMesh1D.a` and `libMesh1D.so` to `$(PACS_LIB_DIR)`

If you use the shared library, ensure the runtime loader can find the installed
libraries, typically through `LD_LIBRARY_PATH` or the rpath settings already
used by the project Makefiles.

## Running The Example

After a successful build:

```bash
./main_testGenerators
```

The program:

- creates a uniform mesh on `[0,10]`
- creates a variable-size mesh on the same interval
- prints the number of nodes and the min/max mesh sizes
- writes node coordinates to `uniform.dat` and `variable.dat`

If `gnuplot` is available you can inspect the output data using the supplied
script or by plotting the files directly.

## Possible Issues And Limitations

The code is generally clear and the sample run works, but there are a few
points worth noting:

- `make exec` alone fails from a clean tree.
  The executable target links against `-lMesh1D`, but it does not force the
  library to be built first.

- The static executable path is incomplete.
  `meshGenerators.cpp` includes `RKF.hpp`, and `RKF.hpp` pulls in Newton
  classes. With `libMesh1D.a`, the final link of `main_testGenerators` does not
  propagate all required Newton symbols automatically, so the recommended
  executable path is the shared-library one.

- `Domain1D` does not enforce `left() <= right()`.
  The class allows reversed endpoints. `length()` remains positive, but some
  generators implicitly assume the interval orientation is meaningful.

- `Mesh1D::hmin()` only checks that at least two nodes are present.
  Unlike `hmax()`, it does not explicitly validate that the node sequence is
  strictly increasing before returning the minimum spacing. In normal usage the
  generators produce ordered nodes, but the class itself does not enforce that
  invariant if a different generator is introduced.

- `VariableSize` requires a strictly positive spacing function.
  If the spacing function becomes zero or negative anywhere in the interval,
  mesh generation fails by design.

## What You Learn Here

- a compact implementation of the strategy pattern
- basic encapsulation of a one-dimensional mesh and interval domain
- generation of adapted meshes from a target spacing law
- reuse of an ODE solver as a building block for mesh construction
