# The CMake system of the examples.

Each example has a `CMakeLists.txt` that allows you to compile the code and/or produce libraries, depending on the case.

All CMake files follow the same structure (with a few exceptions). They:

- Include the file `cmake/PacsCommon.cmake` in the `Examples` root folder, which contains the common configuration.
- Include the file `cmake/PacsExample.cmake`, which provides helper functions to build executables and libraries in a consistent way.
- Optionally set per-example options (MPI/OpenMP/TBB/Boost/HDF5/BLAS) and overrides (C++ standard, extra flags, extra include paths).

## The PacsCommon.cmake file

`cmake/PacsCommon.cmake` is the shared configuration that replaces the old `Makefile.inc`. It:

- Determines `PACS_ROOT` (the root of the Examples tree). You can override it with `-DPACS_ROOT=/path/to/Examples`.
- Sets a default C++ standard (`C++23`) and warning flags.
- Configures the default install prefix so that `cmake --install` writes into `Examples/include` and `Examples/lib`.
- Provides helper functions that add common include paths and link optional dependencies.

## The PacsExample.cmake file

`cmake/PacsExample.cmake` provides helper functions used by most examples:

- `pacs_add_example_dir(...)` scans the current directory, creates executables from `main*.cpp`, and treats the remaining sources as support code.
- If `LIB_NAME` is provided, it builds a library (static and/or shared) and links executables against it.
- If `PYTHON_MODULE_NAME` is provided, it builds a Python extension module from the non-main sources.
- It also installs headers from the local directory into `Examples/include`.

## Typical build commands

From any example directory:

```bash
cmake -S . -B build
cmake --build build
cmake --install build
```

In-source builds are also allowed (use `-B .`), but out-of-source builds are recommended.

## Common options

You can pass configuration options as CMake cache variables, for example:

- `-DPACS_ROOT=/path/to/Examples`
- `-DEIGEN_DIR=/path/to/eigen3`

Some examples require optional dependencies (MPI, OpenMP, TBB, Boost, HDF5, BLAS, muparser). These are enabled in the corresponding `CMakeLists.txt` files and will be discovered by CMake using `find_package()`.
