# FactoryPlugin — Detailed explanation

This folder demonstrates a minimal plugin system that uses dynamic libraries and a simple factory map to register and create objects (Polygons). The approach shows how to automatically register types when a shared object is loaded at runtime so the main program can create instances without compile-time knowledge of concrete types.

Files of interest
- `main_poly.cpp` — example program that loads plugins and queries the factory.
- `FactoryTraits.hpp` / `FactoryTraits.cpp` — declares/defines the shared `polyFactory` map and builder types.
- `Polygon.hpp` / `Polygon.cpp` — base class and minimal implementation for Polygon objects.
- `Quadrilateral.hpp` / `Quadrilateral.cpp` — one plugin implementation (registers `Quadrilateral`).
- `TriangleAndSquare.hpp` / `TriangleAndSquare.cpp` — another plugin (registers `Triangle` and `Square`).
- `plugins.txt` — plain list of shared-object filenames to load.
- `loadPolyFactory.hpp` — alias for `apsc::LoadLibraries` used as `loadPolyFactory`.
- `../LoadLibraries/*` (shared in project) — implementation of the dynamic loader `apsc::LoadLibraries` (uses `dlopen`/`dlclose`).

Design overview

The plugin mechanism is a classic pattern combining:

1. A global factory container accessible by both the main program and plugin libraries (`polyFactory`).
2. A runtime loader that opens shared libraries by filename (`apsc::LoadLibraries`).
3. Automatic registration code inside each plugin library that runs when the library is loaded (constructor attribute), which inserts builder entries into `polyFactory`.
4. The main program simply loads plugins and then queries `polyFactory` by a string id to construct objects.

Key types

- `using PolyBuilder = std::function<std::unique_ptr<Geometry::Polygon>()>`
  - A callable that returns a newly-allocated polymorphic `Polygon`.
- `using PolyFactory = std::map<PolyId, PolyBuilder>`
  - The factory map keyed by string identifiers (like "Triangle").
- `extern PolyFactory polyFactory;` (declared in `FactoryTraits.hpp`, defined in `FactoryTraits.cpp`)
  - The single, shared factory instance used at runtime.
- `using loadPolyFactory = apsc::LoadLibraries;` (alias in `loadPolyFactory.hpp`)
  - Convenience name for the loader object used by `main_poly.cpp`.

How automatic plugin registration works (step-by-step)

1. The main program constructs an instance of `loadPolyFactory` with a file name `plugins.txt`:
   - `loadPolyFactory loadPlugins("plugins.txt");`
   - The `LoadLibraries` constructor reads the file and calls `dlopen` on each listed `.so` file (via `loadSingleLibrary`).

2. When `dlopen()` successfully opens a shared object, the dynamic loader (ld.so) runs the library initializers for that shared object. In this code the plugins use a GNU extension:
   - Each plugin defines a function inside an anonymous namespace with the GCC/Clang attribute `__attribute__((constructor))`, e.g.:
     ```cpp
     namespace {
       void __attribute__((constructor)) LoadF() {
         polyFactory["Triangle"] = [](){ return std::make_unique<Triangle>(); };
         polyFactory["Square"]   = [](){ return std::make_unique<Square>(); };
       }
     }
     ```
   - The `constructor` attribute marks `LoadF()` to be run automatically when the shared object is loaded. That call inserts builder lambdas into the global `polyFactory` map.

3. Because `polyFactory` is an `extern` symbol declared in the plugin headers and defined in `FactoryTraits.cpp` (linked into the main program or a common shared library), the builder lambdas inserted by the plugin modify the same `polyFactory` instance the main program later queries.

4. After all libraries are loaded, the main program can look up a string key and call the builder:
   ```cpp
   auto where = polyFactory.find(answer);
   if (where != polyFactory.end()) {
     auto thePoly = (where->second)(); // calls the builder lambda -> unique_ptr<Polygon>
     thePoly->showMe(cout);
   }
   ```

The `apsc::LoadLibraries` implementation (what it does)

- `LoadLibraries::load(fileName)` reads `plugins.txt`, extracts non-blank tokens from each line, and calls `loadSingleLibrary(libName)`.
- `LoadLibraries::loadSingleLibrary(libName, mode)` calls `dlopen(libName.c_str(), mode)` and stores the returned handle in an internal `std::unordered_map<std::string, void*> loadedLibs`.
- The destructor (RAII) calls `close()` which iterates `loadedLibs` and calls `dlclose` on each handle.
- `getLibraryHandle(libName)` returns the dl handle for advanced use (e.g., `dlsym` if you need to query specific symbols yourself).

Important practical details, pitfalls and portability notes

- Symbol visibility / where `polyFactory` lives
  - Plugins expect to find the `polyFactory` symbol at runtime. That symbol must be visible to the plugins:
    - Either define `polyFactory` in a shared library that both main program and plugins link against, or
    - Define `polyFactory` in the main executable and ensure symbols from the main are available to dynamically loaded libraries (e.g. link the main with `-rdynamic` on GNU toolchains so exported symbols are visible).
  - The comment in `FactoryTraits.hpp` hints: "must be stored in a shared lib" — that is one robust way to ensure a single shared instance across main and plugins.

- Automatic registration uses GCC/Clang `__attribute__((constructor))`
  - This is a convenient, portable-to-POSIX/gcc/clang technique but is not standard C++.
  - On other toolchains/compilers you may need alternatives (e.g., define an init function with a known symbol and call it via `dlsym` after loading the library; or use platform-specific pragma or Windows DllMain).

- Order and lifetime
  - Because `LoadLibraries` closes libraries in its destructor (RAII), any objects (code or data) provided by a shared object must not be used after its `dlclose`. In `main_poly.cpp` the comment warns that clearing the factory or letting `LoadLibraries` go out of scope too early will leave `polyFactory` entries that refer to types implemented in unloaded libraries — that will cause undefined behaviour.
  - The `LoadLibraries` instance must live at least as long as you need to construct and use plugin-provided objects, and you must not keep plugin objects alive past `dlclose`.

- Error handling
  - `LoadLibraries::loadSingleLibrary` checks `dlopen` return and prints `dlerror()` on failure; the program uses the returned `bool` to detect problems.

- Thread-safety
  - The simple `polyFactory` map and `LoadLibraries` are not protected by mutexes. If you load libraries or register from multiple threads, you should add synchronization.

- Alternative: explicit init function
  - If you want a portable approach, plugins can export a well-known symbol (e.g. `extern "C" void register_plugin()`), and the loader can `dlsym` that symbol after `dlopen` and call it. This avoids reliance on `constructor` attribute and gives deterministic control over registration order.

Example build/runtime flow

1. Build plugins as shared libraries (`libTS.so`, `libQuad.so`) and place them in the folder containing `plugins.txt` or provide relative/absolute paths in `plugins.txt`.
2. Run the example program `main_poly` (linked with `FactoryTraits.cpp` so `polyFactory` is defined and visible). The program constructs `loadPolyFactory("plugins.txt")` which `dlopen`s each library.
3. Each plugin's `LoadF()` runs and registers builder lambdas in `polyFactory`.
4. The program prompts the user for shape names, finds the builder in `polyFactory` and calls it to get an instance.

Summary

This folder implements a concise, effective plugin mechanism:
- `LoadLibraries` handles opening/closing shared objects.
- Plugins register themselves automatically when loaded using `__attribute__((constructor))` functions that insert builder lambdas into a global `polyFactory`.
- The main program just loads plugins and uses `polyFactory` to create objects by name.

If you want, I can also:
- Add a `register_plugin()` symbol + `dlsym` example to make registration portable, or
- Add a short `README` showing compilation commands and the `-rdynamic`/export-symbols steps needed to make `polyFactory` visible to plugins on Linux, or
- Generate an example of how to `dlsym` a specific symbol from a loaded library.

Which follow-up would you like?