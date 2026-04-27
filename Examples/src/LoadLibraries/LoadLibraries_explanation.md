# `LoadLibraries`: detailed explanation

This folder contains a small example of a plugin-like architecture based on
runtime loading of shared libraries. The central idea is simple:

1. the program reads a list of shared objects from a text file;
2. it opens them with `dlopen()`;
3. each shared object registers some functions in a common factory as soon as
   it is loaded;
4. the main program retrieves the registered functions by name and executes
   them.

The example is intentionally compact, but it shows several important concepts:
resource management with RAII, symbol registration through shared state, and
the lifetime issues that arise when code is unloaded dynamically.

## Files in the folder

### `LoadLibraries.hpp`

This header defines the class `apsc::LoadLibraries`, which is the small utility
used to manage dynamically loaded libraries.

Its responsibilities are:

- store the handles returned by `dlopen()`;
- provide functions to load libraries one at a time or from a file;
- close individual libraries or all loaded libraries;
- release all opened handles automatically in the destructor.

The class is moveable but not copyable because it owns raw library handles.
Conceptually, it behaves like a repository of open shared libraries.

### `LoadLibraries.cpp`

This file implements the loader.

The relevant member functions are:

- `load(std::string fileName, int mode)`: opens the file containing the list of
  libraries and tries to load them one by one;
- `loadSingleLibrary(std::string libName, int mode)`: calls `dlopen()` on a
  single library and stores the resulting handle in the internal map;
- `close()`: calls `dlclose()` on all stored handles and clears the map;
- `close(std::string libName)`: closes only one named library;
- `getLibraryHandle(std::string libName) const`: returns the raw handle, if
  present.

Internally, the class stores

```cpp
std::unordered_map<std::string, void *> loadedLibs;
```

so the key is the library name and the value is the raw `dlopen()` handle.

### `libtraits.hpp`

This header defines the types shared by the example plugins and by the main
program.

It introduces:

- `loadlibraries::FunType`, which is a function object with signature
  `double(std::vector<double> const &)`;
- `loadlibraries::FunFactory`, which is a specialization of the generic
  `FunctionFactory` template;
- `loadlibraries::funFactory`, declared as an external object.

The idea is that all dynamically loaded libraries will register callable
objects into the same factory instance, and `main.cpp` will later query that
factory.

### `libFactory.cpp`

This file defines the shared factory object:

```cpp
FunFactory& funFactory = FunFactory::Instance();
```

This is a key part of the example. The plugins do not return function pointers
directly to the caller. Instead, when they are loaded, they insert named
functions into this common factory. The main program and the plugins therefore
communicate indirectly through shared registration state.

### `lib1.cpp`

This file implements one example shared library.

It defines two functions:

- `norm2`, the Euclidean norm;
- `norminf`, the infinity norm, implemented as `max_i |x_i|`.

At the end of the file there is a function marked with
`__attribute__((constructor))`. This attribute tells the system to execute that
function automatically when the shared library is loaded with `dlopen()`.

The constructor function registers:

- `"norm2"` mapped to `norm2`;
- `"norminf"` mapped to `norminf`.

### `lib2.cpp`

This file implements a second example shared library.

It defines:

- `norm1`, the 1-norm, that is the sum of the absolute values;
- `norm0`, which in this example counts how many entries are not zero.

As in `lib1.cpp`, the library has a constructor function that is run
automatically at load time. That function registers:

- `"norm1"` mapped to `norm1`;
- `"norm0"` mapped to `norm0`.

### `main.cpp`

This is the driver program that shows how the mechanism works in practice.

It does not call `dlsym()`. Instead, it relies on automatic registration:
loading the libraries causes them to populate `funFactory`, and the program
then uses the factory to discover and execute the available functions.

### `libraries.txt`

This file contains the list of libraries that `LoadLibraries` will load. In the
current example it contains

```text
./lib1.so
./lib2.so
```

Each line is read as a library name or path and passed to `dlopen()`.

### `Makefile`, `Makefile.inc`, `README.md`

These files support compilation and provide short usage notes. The Makefile
builds:

- the support shared libraries `lib1.so`, `lib2.so`, and `libFactory.so`;
- the loader library `libloadlibs.so`;
- the example executable `main`.

## How the loader works

The class `LoadLibraries` is a small RAII wrapper around `dlopen()` and
`dlclose()`.

### Loading from a file

When `load("libraries.txt")` is called, the file is read line by line. For each
line, the code:

1. extracts the first non-blank token;
2. treats that token as a library name;
3. passes it to `loadSingleLibrary()`.

Blank lines are ignored. Loading stops if one library fails to load.

### Loading a single library

`loadSingleLibrary()` first checks whether that library name is already present
in `loadedLibs`. If it is not, it calls:

```cpp
dlopen(libName.c_str(), mode);
```

If `dlopen()` succeeds, the returned handle is stored in the map. If it fails,
the code prints the diagnostic from `dlerror()`.

### Closing libraries

The class provides two ways to unload:

- close one specific library by name;
- close all currently loaded libraries.

The destructor calls `close()`, so cleanup also happens automatically when the
`LoadLibraries` object goes out of scope.

## What happens when a plugin is loaded

The crucial mechanism in this example is the constructor attribute in
`lib1.cpp` and `lib2.cpp`.

When `dlopen("./lib1.so", ...)` succeeds, the dynamic loader runs the library's
constructor function automatically. That function performs the registration:

```cpp
funFactory.add("norm2", FunType{norm2});
funFactory.add("norminf", FunType{norminf});
```

The same happens for `lib2.so`, which adds `norm1` and `norm0`.

As a consequence, the main program does not need explicit knowledge of which
functions each library exports. It only needs to know that loading the library
causes registration into the common factory.

## Detailed walkthrough of `main.cpp`

The `main()` function is short, but each line is doing something important.

### 1. Select the relevant namespaces and types

At the beginning:

```cpp
using namespace loadlibraries;
using apsc::LoadLibraries;
```

This makes the plugin factory types available and imports the loader class.

### 2. Load the libraries listed in `libraries.txt`

The line

```cpp
LoadLibraries libraries("libraries.txt");
```

constructs a loader object and immediately asks it to read `libraries.txt`.

In the current setup, that means:

1. `./lib1.so` is loaded;
2. `./lib2.so` is loaded.

During each `dlopen()`, the corresponding constructor function runs, so the
factory is populated as a side effect of loading.

At this point `funFactory` already contains the functions registered by both
libraries.

### 3. Print a status message

The program prints

```cpp
std::cout << "Libraries loaded" << std::endl;
```

This is only diagnostic output confirming that the loading phase is complete.

### 4. Ask the factory which functions are available

The next statement is

```cpp
auto registeredFunctions = funFactory.registered();
```

This retrieves the names currently registered in the factory. Given the current
two plugins, the list contains four entries:

- `norm0`
- `norm1`
- `norm2`
- `norminf`

The exact iteration order depends on the factory implementation and should not
be interpreted as semantically meaningful.

### 5. Build the input vector used for the demo

The example then creates

```cpp
std::vector x{1., 0., 3., 4., 5., 6., -9.};
```

This vector is passed to every registered function so that the output of the
different norms can be compared on the same data.

For this specific vector:

- `norm0(x)` returns `1`, because there is one zero entry;
- `norm1(x)` returns `28`;
- `norm2(x)` returns `sqrt(168)`, approximately `12.9615`;
- `norminf(x)` returns `9`.

### 6. Retrieve each function from the factory and execute it

The central loop is:

```cpp
for(auto fun : registeredFunctions)
{
  FunType f;
  f = funFactory.get(fun);
  auto result = f(x);
  std::cout << fun << "(x)=" << result << std::endl;
}
```

What this does is:

1. take one registered name, for example `"norm2"`;
2. ask the factory for the corresponding callable object;
3. invoke that callable on the sample vector `x`;
4. print the result.

This is the main didactic point of the example: the executable does not know at
compile time which concrete functions it will call. It discovers them at
runtime from the set of loaded libraries.

### 7. Clear the factory before unloading the libraries

Before the libraries are closed, the code executes:

```cpp
funFactory.clear();
```

This is an important lifetime precaution. The factory stores callable wrappers
that ultimately refer to code living inside the shared libraries. If the
libraries were unloaded first, the factory could retain handles to code that no
longer exists.

So the intended order is:

1. remove the registered callables from the factory;
2. unload the shared libraries.

### 8. Close the libraries

Finally, the program does:

```cpp
libraries.close();
```

This explicitly unloads the libraries. Even if this line were omitted, the
destructor of `libraries` would close them automatically at the end of `main()`.

The explicit call is useful here because it makes the intended cleanup order
visible in the example.

## Why this example is useful

This folder demonstrates a common pattern used in plugin systems:

- a core application loads extension modules at runtime;
- the modules self-register into a common registry;
- the application discovers capabilities dynamically;
- cleanup must respect object and code lifetime.

The example is intentionally minimal, but the architecture scales to more
realistic settings where plugins register algorithms, solvers, boundary
conditions, post-processing tools, or application-specific commands.

## Important limitations of the example

This example is good for teaching, but it is deliberately simple.

- It uses GCC/Clang-specific `__attribute__((constructor))`.
- It does not use `dlsym()` to resolve explicit entry points.
- It assumes the main program and the plugins all refer to the same factory
  instance.
- It relies on manual discipline for cleanup order.
- It does not add synchronization, so it is not designed for concurrent plugin
  loading.

Those limitations are acceptable here because the goal is to show the mechanism
clearly rather than to build a full production plugin framework.
