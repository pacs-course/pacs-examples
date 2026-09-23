# Automatic differentiation: a C++ example

This directory contains a small C++17 example project illustrating the `autodiff` package with Eigen.

The example is now organized into two separate programs:

- a forward-mode executable, showing scalar derivatives, second derivatives, and
  forward-mode gradients/Jacobians;
- a reverse-mode executable, showing scalar partial derivatives and reverse-mode
  gradient/Hessian computations.

The forward- and reverse-mode sections use the common running example
`f(x,y)=(x+y)exp(xy)` and show every tangent and adjoint update numerically at
`x=1`, `y=2`.

## Files

- `autodiff_demo.cpp`: forward-mode autodiff example, with short explanatory lines in the program output.
- `autodiff_reverse_demo.cpp`: reverse-mode autodiff example, with short explanatory lines in the program output.
- `CMakeLists.txt`: CMake project building both executables.
- `run_autodiff_demos.sh`: convenience script that builds and runs both demos.
- `Makefile`: a makefile following the layout of the makefiles of the examples of the course (alternative to cmake).


## Dependencies

Install Eigen 3, **version 5.x**, and use the `autodiff` C++ library provided with the examples.
If you use CMake, the library must be installed with its CMake package configuration so that the imported target
`autodiff::autodiff` is available. The `Makefile` is already set up to use the available version.
*Note that the autodiff package provided with the examples has been ported to Eigen3 version 5; the official version on GitHub is still not compatible (as of August 2026).*

## Build and run the examples

**With CMake**
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/autodiff_forward_demo
./build/autodiff_reverse_demo
```

The project also provides a convenience launcher script that builds both
executables and runs them in sequence:

```sh
./run_autodiff_demos.sh
```

If CMake cannot locate `autodiff`, add its installation prefix to
`CMAKE_PREFIX_PATH`, for example:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/autodiff/prefix
```

**Using the MAkefile directly**

```sh
make all
```

## Notes


The code uses the API documented at the official `autodiff` project site:
<https://autodiff.github.io/>.

The two executables are intentionally separate so that each one uses only the
relevant autodiff mode. This makes the code easier to read and avoids the need
for mode-switching logic or wrappers when the forward and reverse APIs are mixed
in one translation unit.

The program output is intentionally instructional: after each numerical result,
it prints a short sentence explaining what the example is illustrating.

## What do I learn here?

- The basic usage of a well known automatic differentiation tool

