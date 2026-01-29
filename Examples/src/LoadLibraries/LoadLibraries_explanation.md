# LoadLibraries — Detailed explanation

This folder contains a small utility library and example that demonstrates loading shared libraries at runtime (plugins) using the POSIX `dlopen`/`dlclose` API and registering functions exposed by those libraries in a generic factory. The design focuses on simple, safe handling of dynamic libraries via RAII and on a straightforward mechanism for plugin registration using library constructors.

Files of interest
- `LoadLibraries.hpp` / `LoadLibraries.cpp` — the loader class using `dlopen` and keeping handles in a map. Implements load-from-file, load-single-library, close, and get-handle operations.
- `main.cpp` — example program that uses `LoadLibraries` and a small `FunctionFactory` to execute functions exported by the loaded libraries.
- `libtraits.hpp` / `libFactory.cpp` — glue that defines the `FunFactory` type and provides the shared factory instance.
- `lib1.cpp`, `lib2.cpp` — example plugin libraries exposing functions by registering them with `funFactory` in a constructor function.
- `libraries.txt` — list of shared-object files to load (one per line).
- `Makefile` / `README.md` — build/run notes.

High-level design and methodology

1. Dynamic loading with RAII
   - `apsc::LoadLibraries` wraps the `dlopen`/`dlclose` calls and keeps loaded handles in an internal `std::unordered_map<std::string, void*> loadedLibs`.
   - The `LoadLibraries` destructor calls `close()` to `dlclose` any opened handles (RAII): libraries are closed automatically when the loader object is destroyed.
   - The loader exposes `load(fileName)` to read a file with library names and `loadSingleLibrary(libName)` to open an individual library and add it to `loadedLibs`.

2. Plugin registration via constructors
   - Each plugin (e.g., `lib1.cpp`, `lib2.cpp`) declares some functions and registers them in a shared factory `funFactory` by using a constructor function annotated with `__attribute__((constructor))`.
   - Example in `lib1.cpp`:
     ```cpp
     __attribute__((constructor)) void load() {
       funFactory.add("norm2", FunType{norm2});
       funFactory.add("norminf", FunType{norminf});
     }
     ```
   - The constructor function runs automatically when the shared object is loaded with `dlopen`, inserting function wrappers into the global factory.

3. Shared factory for exported symbols
   - `libtraits.hpp` defines types used for the example: `FunType` (callable signature) and `FunFactory` (a `FunctionFactory` from `Factory.hpp`).
   - `libFactory.cpp` defines `FunFactory& funFactory = FunFactory::Instance();` — a global factory instance accessible by both the main program and the dynamically loaded plugins (they all link to the same symbol instance).
   - When plugin constructor functions call `funFactory.add(...)`, the main program later queries the factory to obtain std::function wrappers that call the plugin functions.

Step-by-step runtime flow

1. The main program constructs a `LoadLibraries` instance with the name of a file listing libraries (e.g., `LoadLibraries libraries("libraries.txt");`).
   - `LoadLibraries::load()` reads the file line-by-line (trimming blanks) and calls `loadSingleLibrary(libName)` for each entry.

2. For each library file name, `loadSingleLibrary` calls `dlopen(libName.c_str(), mode)` and stores the returned handle in `loadedLibs`.
   - `dlopen` runs library initializers which call the plugins' constructor functions. Those constructors register functions into `funFactory`.

3. After libraries are loaded, the main code queries `funFactory.registered()` to get the list of registered names and calls `funFactory.get(name)` to retrieve a `FunType` callable for each registered function. The example then invokes these callables on a sample vector.

4. Before closing the libraries the main code clears `funFactory` (factory entries) to ensure no pointers to plugin code remain. Finally, it calls `libraries.close()` or relies on the `LoadLibraries` destructor to `dlclose` the libraries.

Important implementation details

- `LoadLibraries::load` reads `fileName`, extracts non-blank tokens from each line and calls `loadSingleLibrary`. It prints a message for empty lines.
- `LoadLibraries::loadSingleLibrary` checks if the library is already present in `loadedLibs`; if not it calls `dlopen` and stores the handle; on failure it prints `dlerror()` and returns false.
- `LoadLibraries::close()` iterates loaded handles and calls `dlclose`; `close(libName)` closes a single library by name.
- `LoadLibraries::getLibraryHandle(libName)` returns the raw `void*` handle (useful for `dlsym` or advanced operations).

Safety, lifetime and pitfalls

- Lifetime coupling: plugin objects and function pointers often refer to code/data in the loaded shared object. After `dlclose`, those addresses become invalid. Therefore:
  - Keep the `LoadLibraries` instance alive as long as you need plugin-provided code/data.
  - Do not keep callable wrappers (or objects) around after you `dlclose` the library; clear factory entries first.

- Symbol visibility: the plugins and the main program must agree on where the shared factory `funFactory` is defined. The typical patterns are:
  - Define `funFactory` in a shared library that both the main program and plugins link against.
  - Or define it in the main executable and ensure exported symbols are visible to dlopen-ed libraries (on Linux use `-rdynamic` when linking the main executable so the symbol is exported).
  - In this project `libFactory.cpp` creates `funFactory` using `FunFactory::Instance()`; ensure build/link settings make this symbol available to plugins.

- Constructor attribute portability:
  - The code uses GCC/Clang `__attribute__((constructor))` to run registration code at `dlopen` time. This is convenient but not standard C++.
  - Portable alternative: export a known C function (e.g. `extern "C" void register_plugin()`), and after `dlopen` call `dlsym(handle, "register_plugin")` and call it explicitly.

- Error handling and diagnostics:
  - `LoadLibraries` prints `dlerror()` on `dlopen` failure. You may want to propagate errors or throw exceptions in library code depending on how critical plugin loading is.

- Threading:
  - Neither `funFactory` nor `LoadLibraries` use internal synchronization. If loading plugins concurrently or registering from multiple threads, add mutexes.

Example usage (main.cpp flow)

1. Build shared libs (`lib1.so`, `lib2.so`) and place them relative to `libraries.txt` entries.
2. Run the test program. It will load each .so, plugin constructors register function names (`norm2`, `norminf`, `norm1`, `norm0`) into `funFactory`.
3. The example obtains the list of registered functions and executes each on a sample vector.

Commands (example, from inside `src/LoadLibraries`)

```bash
# build shared libraries and example (project uses provided Makefile targets)
make alllibs
make exec
./main       # or the produced test executable
```

Potential improvements and alternatives

- Replace constructor-based registration with an explicit `register_plugin` symbol + `dlsym` for portability and control.
- Add logging and more robust error propagation instead of printing to `stderr`.
- Make `LoadLibraries` thread-safe.
- Allow versioning or plugin metadata parsing to avoid loading incompatible plugins.

Conclusion

The `LoadLibraries` folder implements a lightweight, clear demonstration of runtime plugin loading and registration via `dlopen` and constructor attributes. It is suitable for didactic purposes and small projects but should be adapted (explicit registration, symbol visibility checks, thread-safety) for production-level plugin systems.

---

If you want, I can:
- Add a `dlsym`-based registration example (modify a plugin to provide `register_plugin()` and update `LoadLibraries::loadSingleLibrary` to call it), or
- Add a short README snippet with exact build commands (Makefile commands) tailored to your environment. Which would you like next?