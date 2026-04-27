# FactoryPluginII

This folder contains a second plugin example based on a polymorphic hierarchy
of geometric objects. As in `FactoryPlugin`, the main program loads shared
libraries at runtime and creates objects through a common factory.

The important difference is the registration technique:

- `FactoryPlugin` uses functions marked with `__attribute__((constructor))`;
- `FactoryPluginII` uses namespace-scope static objects whose constructors
  register the builders.

So this folder demonstrates a more standard C++ style of automatic
registration, without relying on the GNU constructor attribute.

## Folder contents

- `Polygon.hpp`, `Polygon.cpp`
  Define the base class of the polygon hierarchy.

- `TriangleAndSquare.hpp`, `TriangleAndSquare.cpp`
  Define `Triangle` and `Square` and register them into the shared factory
  through a static local registration object when `libTS.so` is loaded.

- `Quadrilateral.hpp`, `Quadrilateral.cpp`
  Define `Quadrilateral` and register it into the shared factory through a
  static local registration object when `libQuad.so` is loaded.

- `FactoryTraits.hpp`, `FactoryTraits.cpp`
  Define the factory-related types and the shared global object `polyFactory`.

- `loadPolyFactory.hpp`
  Defines the alias used to load the plugin libraries listed in `plugins.txt`.

- `main_poly.cpp`
  Example driver that loads the plugins, queries the factory, and creates the
  requested polygon.

- `plugins.txt`
  Lists the shared libraries loaded at runtime.

## How the example works

The shared libraries `libTS.so` and `libQuad.so` contain namespace-scope static
objects of local helper types named `LoadF`. When a shared library is loaded,
those objects are constructed and their constructors insert builders into the
shared map `polyFactory`.

As a result:

- loading `libTS.so` registers `"Triangle"` and `"Square"`;
- loading `libQuad.so` registers `"Quadrilateral"`.

The main program never constructs these classes directly. It only looks up a
name in `polyFactory` and, if found, calls the associated builder.

## Compilation

From inside `Examples/src/Plugins/FactoryPluginII`, build the example with:

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
- `libloadlibs`, from `Examples/lib`, which provides the dynamic-library
  loader.

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

If the name is registered, the program calls the corresponding builder and the
created object prints its description through `showMe()`.

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
illustrate the registration mechanism rather than provide a complete plugin
framework.

## What this example teaches

- how to combine a polymorphic hierarchy with a runtime factory;
- how shared libraries can self-register products through static objects;
- how a program can discover and create runtime-selected objects by name;
- how automatic registration can be implemented in standard C++ without
  `__attribute__((constructor))`.
