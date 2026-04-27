# LoadLibraries

This folder contains a small utility library and a self-contained example that
shows how to load shared libraries dynamically and keep their handles in a
common repository.

The main component is `apsc::LoadLibraries`, a lightweight RAII wrapper around
`dlopen()` and `dlclose()`. It allows you to:

- load one shared library at a time;
- load several libraries listed in a text file;
- store the corresponding handles in an internal container;
- close all loaded libraries automatically when the object goes out of scope.

The example program in `main.cpp` uses this loader together with a function
factory. When `lib1.so` and `lib2.so` are loaded, they automatically register
their functions into the shared factory, and `main.cpp` retrieves and executes
them.

## Build targets

The directory provides several Makefile targets.

- `make dynamic`
  Builds the dynamic loader library `libloadlibs.so`, the support libraries
  `lib1.so`, `lib2.so`, `libFactory.so`, and the example executable `main`.

- `make static`
  Builds the static version of the loader library and then builds the example
  executable.

- `make alllibs`
  Builds both the static and dynamic versions of the loader library.

- `make exec`
  Builds the support libraries and the example executable.
  This target assumes that the loader library has already been built, so in
  practice `make dynamic` is the simplest target to use when you want to run
  the example from scratch.

- `make clean`
  Removes object files, the executable, and the support shared libraries built
  in this folder.

- `make distclean`
  Performs a deeper cleanup and also removes the generated loader libraries and
  dependency files.

## Recommended compilation sequence

From inside `Examples/src/LoadLibraries`, the most direct way to compile and
run the example is:

```bash
make dynamic
./main
```

This produces:

- `libloadlibs.so`, the dynamic library implementing `LoadLibraries`;
- `lib1.so` and `lib2.so`, the example plugins;
- `libFactory.so`, which provides the shared factory instance;
- `main`, the example executable.

If you also want the static library version of the loader, use:

```bash
make alllibs
```

## Installation

The target

```bash
make install
```

installs the `LoadLibraries` tool into the common `Examples` directories used
by the repository build system.

More precisely, it copies:

- `LoadLibraries.hpp` into `Examples/include`;
- `libloadlibs.a` and `libloadlibs.so` into `Examples/lib`.

Therefore `make install` makes the library for loading shared libraries
dynamically available in the common `lib` folder of `Examples`, so that other
examples in the repository can link against it.

In practice, a typical sequence is:

```bash
make alllibs
make install
```

or, if you only need the shared library,

```bash
make dynamic
make install
```

## A note on lifetime

The loader object must stay alive for as long as the program uses symbols,
functions, or objects coming from the loaded shared libraries. Once the
libraries are closed, those symbols are no longer valid.

In this example, `main.cpp` first clears the factory and only afterwards closes
the libraries. That order is important because the factory stores callable
objects that ultimately refer to code living inside the shared libraries.

## What you learn from this example

This example shows a simple plugin architecture based on:

- runtime loading of shared libraries;
- automatic registration of functionality at load time;
- centralized storage of library handles;
- controlled cleanup through RAII.

For a more detailed explanation of the source files and of what `main.cpp`
does step by step, see `LoadLibraries_explanation.md`.
