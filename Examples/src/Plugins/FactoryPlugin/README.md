# FactoryPlugin

This folder contains a small plugin example based on a polymorphic hierarchy of
geometric objects. The goal is to show how dynamically loaded shared libraries
can register builders into a common factory, so that the main program can
create objects selected at runtime.

The example combines three ingredients:

- a base class hierarchy (`Polygon`, `Triangle`, `Square`, `Quadrilateral`);
- a shared factory mapping names to builder functions;
- dynamic loading of plugin libraries through `LoadLibraries`.

## Folder contents

- `Polygon.hpp`, `Polygon.cpp`
  Define the abstract base class used by the example hierarchy.

- `TriangleAndSquare.hpp`, `TriangleAndSquare.cpp`
  Define the `Triangle` and `Square` classes and register them into the shared
  factory when `libTS.so` is loaded.

- `Quadrilateral.hpp`, `Quadrilateral.cpp`
  Define the `Quadrilateral` class and register it when `libQuad.so` is loaded.

- `FactoryTraits.hpp`, `FactoryTraits.cpp`
  Define the factory-related types and the shared global object `polyFactory`.

- `loadPolyFactory.hpp`
  Defines the alias used to load the plugin libraries listed in `plugins.txt`.

- `main_poly.cpp`
  Example driver that loads the plugins, queries the factory, and creates the
  requested polygon.

- `plugins.txt`
  Lists the shared libraries to load at runtime.

## How the example works

The shared libraries `libTS.so` and `libQuad.so` contain constructor functions
marked with `__attribute__((constructor))`. When the libraries are loaded with
`dlopen()`, those constructor functions run automatically and insert builders
into the shared map `polyFactory`.

As a result:

- loading `libTS.so` registers `"Triangle"` and `"Square"`;
- loading `libQuad.so` registers `"Quadrilateral"`.

The main program does not build these objects directly. It only asks the
factory whether a given name is registered and, if so, calls the associated
builder.

## Compilation

From inside `Examples/src/Plugins/FactoryPlugin`, build the example with:

```bash
make
```

This builds:

- `libFactory.so`, containing the shared `polyFactory` object;
- `libTS.so`, the plugin library for `Triangle` and `Square`;
- `libQuad.so`, the plugin library for `Quadrilateral`;
- `main_poly`, the example executable.

The executable is linked against:

- `libFactory.so`, which provides the shared factory object;
- `libloadlibs`, from `Examples/lib`, which provides the dynamic-library loader.

## Running the example

Run:

```bash
./main_poly
```

The program first loads the plugins listed in `plugins.txt`. It then repeatedly
asks for one of the registered polygon names:

- `Quadrilateral`
- `Triangle`
- `Square`
- `END`

If the name is registered, the corresponding builder is called and the created
object prints its description through `showMe()`.

## Prerequisite

This example depends on the `LoadLibraries` utility. Therefore `libloadlibs`
must already be available in `Examples/lib`.

If needed, build and install it first from `Examples/src/LoadLibraries` with:

```bash
make alllibs
make install
```

## Notes on lifetime

The builders stored in `polyFactory` ultimately refer to code living inside the
plugin libraries. For that reason, in a larger application the factory should
be cleared before unloading the libraries. Otherwise the factory may retain
builders pointing to code that is no longer loaded.

This example keeps the cleanup logic simple because its purpose is to
illustrate the plugin-registration mechanism rather than provide a full plugin
framework.

## What this example teaches

- how to combine a polymorphic hierarchy with a runtime factory;
- how shared libraries can self-register products when loaded;
- how a program can discover and create runtime-selected objects by name;
- how dynamic loading and object factories fit naturally together in a plugin
  architecture.

For a longer explanation of the example, see `FactoryPlugin_explanation.md`.
