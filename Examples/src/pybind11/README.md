# Examples of usage of pybind11 (and nanobind)

`pybind11` is a lightweight header-only library that exposes C++ types in Python and vice versa. In this repository it is mainly used to build Python bindings for existing C++ code and to show typical interoperability patterns between Python, STL containers, and Eigen data structures.

`nanobind` is a newer alternative which is meant to produce more efficient code and be faster to compile.

## Getting pybind11

If you use the module system, `pybind11` is available via

```bash
module load pybind11
```

Alternatively, you can install it with

```bash
pip install pybind11
```

or use the submodule stored in `Extras/pybind11`, which should be downloaded if you run

```bash
git pull --recurse-submodules
```

or clone the repository with the `--recurse-submodules` option.

If you use the local submodule copy, then do

```bash
cd Extras/pybind11
./install_PACS.sh
```

`pybind11` and `nanobind` are hosted on GitHub and can also be installed using `pip install` or `conda install -c conda-forge`.


## What is in this folder

This directory contains both small self-contained examples and wrappers around more realistic PACS code. The subfolders are organized by topic:

- `simple`: the smallest introductory example. It wraps a simple C++ sum function into a Python module and includes both a C++ driver and a Python test script. It is useful as a first example of how a compiled Python extension is built and imported.
- `basicZeroFun`: bindings for zero-finding routines based on existing function templates from `LinearAlgebraUtilities`. It shows explicit template instantiation and how Python callables can be passed to C++ numerical algorithms.
- `basicOptim`: a similar example for optimization-related template functions. It focuses on exporting templated C++ code to Python and on adding Python-friendly argument names and documentation to the bindings.
- `Dictionary`: an example of handling a heterogeneous Python dictionary in C++. It shows when standard C++ containers are not enough and why `py::dict` is needed.
- `numpyEigen`: an example of interoperability between NumPy and Eigen. It demonstrates matrix/vector exchange, overloaded functions, and performance-oriented compilation options.
- `OneDMesh`: bindings for a more structured C++ class hierarchy coming from the `src/OneDMesh` code. It covers more advanced topics such as virtual methods, overloaded methods, iterators, and wrapping an existing compiled library.
- `pybind11_demo`: a compact didactical demo that collects several common binding patterns in one place. It includes a `Makefile`, an example `CMakeLists` setup, and a test script.
- `nanobind_demo`: a parallel example based on `nanobind` instead of `pybind11`. It is included for comparison and to show a modern alternative with a very similar binding style.
- `pybind11_examples`: a collection of focused examples, originally adapted from an external tutorial repository, organized by feature. These are best used as a catalogue of small patterns rather than as one single project.

## Contents of `pybind11_examples`

The `pybind11_examples` subfolder contains a sequence of focused examples, each dedicated to one specific binding technique:

- `01_py-list_cpp-vector`: Python list to `std::vector` conversion.
- `02_py-nested-list_cpp-nested-vector`: nested Python lists to nested C++ vectors.
- `03_numpy-1D_cpp-vector`: one-dimensional NumPy arrays and `std::vector`.
- `04_numpy-2D_cpp-vector`: two-dimensional NumPy arrays represented through C++ vector-based structures.
- `05_numpy-2D_cpp-eigen`: NumPy matrices with Eigen.
- `06_class-numpy-eigen`: exposing a class together with Eigen-based data.
- `07_cpp-overload-scalar`: binding overloaded scalar functions.
- `08_cpp-overload-eigen`: binding overloaded functions involving Eigen types.
- `09_numpy_cpp-custom-matrix`: interoperability with a custom matrix class.
- `10_enum`: exposing C++ enumerations to Python.
- `11_class-parent-child`: class inheritance across the C++/Python boundary.
- `12_crtp`: an example involving CRTP-based C++ code.
- `13_static_cast`: explicit overload disambiguation with `static_cast`.

## Compilation

A compiled Python module is a shared library, so its compilation follows the same general rules used for shared libraries.

The most delicate aspects are:

- the naming convention of the generated Python extension;
- selecting the correct Python header paths;
- adding the proper compiler and linker flags.

In the provided `Makefile`s this is handled by asking Python itself for the correct configuration flags. For instance, assuming the Python configuration command is `python3-config` and you want to create a module named `basicOptim`, the `Makefile` should contain lines similar to the following:

```makefile
MODULENAME=basicOptim # Must be consistent with what is declared in the pybind wrapper
PY_INCLUDES != python3-config --includes
PY_EXT != python3-config --extension-suffix
PY_FLAGS=-fPIC -flto # flto is not strictly needed
PY_MODULE=$(MODULENAME)$(PY_EXT)
CXXFLAGS+=$(PY_FLAGS)
CPPFLAGS+=$(PY_INCLUDES)
PY_LDFLAGS != python3-config --ldflags
LDFLAGS+=-shared -flto
LDLIBS+=$(PY_LDFLAGS)
```

The `-flto` flag is optional, but often useful because it enables link-time optimization. It should be used consistently both during compilation and linking.

You can then simply do

```makefile
$(PY_MODULE): $(PY_OBJS)
```

where `PY_OBJS` contains the object files needed to create the Python module.

## Documentation

The official pybind11 documentation, with many additional examples and reference material, is available at:

https://pybind11.readthedocs.io/en/stable/
