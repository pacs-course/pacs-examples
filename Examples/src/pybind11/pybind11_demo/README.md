# pybind11_demo

This folder contains a minimal **pybind11** example showing how to build a Python extension module from C++.

## Folder content

Typical files in this directory are:

- `*.cpp` / `*.hpp`: C++ source/header files implementing the functions/classes exposed to Python.
- `Makefile`: build instructions to compile the extension directly with `make`.
- `CMakeLists.example`: example CMake configuration file that can be copied/renamed to `CMakeLists.txt` if you prefer a CMake-based workflow.
- `README.md`: this documentation file.

Depending on the specific demo version, generated build artifacts (e.g. object files, shared module `.so`, or build directories) may appear after compilation.

---

## Build with `make`

From this folder, run:

```bash
make
```

This compiles the C++ code and creates the Python extension module.

If a clean target is available in the `Makefile`, you can remove build artifacts with:

```bash
make clean
```

If your environment has multiple Python versions, ensure the `Makefile` uses the intended interpreter and headers/libraries.

---

## Alternative build with CMake

If you want to use CMake instead of invoking `make` directly from the provided `Makefile`:

1. Use `CMakeLists.example` as a template.
2. Copy or rename it to `CMakeLists.txt`.
3. Configure and build:

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

This produces the same kind of Python extension module, but through a CMake-managed build.

