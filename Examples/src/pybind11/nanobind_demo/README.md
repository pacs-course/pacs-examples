# nanobind_demo

This folder contains a small **nanobind** example showing how to expose C++ code as a Python extension module.

The example is intentionally kept self-contained for didactical use: a local vendored copy of the minimal `nanobind` sources needed to build the module is included in this directory, so you can compile the example even if `nanobind` is not installed system-wide on your machine.

## Objectives of the example

This example is meant to show the basic ideas behind C++/Python bindings with `nanobind`. In particular, it demonstrates:

- binding free functions with arguments and default values;
- automatic conversion of common STL containers such as `std::vector` and `std::map`;
- bindings involving `Eigen` vectors and matrices;
- passing NumPy-compatible arrays through `nanobind::ndarray`;
- exposing C++ classes, methods, read/write fields, and read-only properties;
- simple operator overloading;
- binding template-based code through explicit instantiations.

The generated Python module is called `pacs_nb`.

## Folder content

- `bindings_nanobind.cpp`: the actual binding code for the example;
- `Makefile`: direct build of the Python extension with `make`;
- `CMakeLists.txt`: alternative build of the same module with CMake;
- `test_nanobind.py`: a small usage example to test the module from Python;
- `include/nanobind/*`, `include/tsl/*`, `src/nb_combined.cpp`, and related files: the vendored `nanobind` sources used by this example;
- `nanobind_vendor_version.txt`: the upstream commit used for the vendored copy;
- `THIRD_PARTY_nanobind_LICENSE`: `nanobind` license file.

## Why both `make` and CMake are provided

This directory provides **two equivalent build workflows** on purpose:

- `make`: useful to show the actual compiler flags and the minimal ingredients needed to build a Python extension module;
- CMake: useful to show a more scalable workflow that is closer to what is typically used in larger projects.

Both versions are provided for **didactical use**. You only need to use **one** of them to compile the module.

## Build with `make`

From this directory, run:

```bash
make
```

This compiles the module `pacs_nb` using the local vendored `nanobind` files in this folder.

To remove build artifacts:

```bash
make clean
```

## Build with CMake

From this directory, run:

```bash
cmake -S . -B build
cmake --build build
```

This produces the same `pacs_nb` Python module, again using the vendored `nanobind` sources shipped with this example.

## Running the example

After building the module, you can try the small test script:

```bash
python3 test_nanobind.py
```

## If you want to install the full `nanobind` library

This example does **not** require a separate installation of `nanobind`, because a local copy is already included here. However, if you want to use `nanobind` in your own projects, installing the full library is usually the better choice.

According to the official `nanobind` documentation, the common installation options are:

```bash
python -m pip install nanobind
```

or, in a Conda environment:

```bash
conda install -c conda-forge nanobind
```

Another common option is to add `nanobind` to your own repository as a Git submodule.

Official references:

- Documentation: https://nanobind.readthedocs.io/en/latest/
- Installation guide: https://nanobind.readthedocs.io/en/latest/installing.html
- Source repository: https://github.com/wjakob/nanobind

If you install the full library, your own CMake-based projects can use the official `nanobind` package instead of vendoring the sources manually.
