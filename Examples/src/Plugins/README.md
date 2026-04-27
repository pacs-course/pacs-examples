# Plugins

This directory contains two small examples of a plugin architecture built on
top of a polymorphic class hierarchy and a runtime factory.

In a plugin architecture, the main program does not need to know all concrete
types at compile time. Instead, it loads shared libraries at runtime and those
libraries extend the program by registering new functionality. This approach is
useful when you want to:

- add new features without recompiling the main executable;
- keep the core program independent from optional components;
- select capabilities dynamically at runtime.

In these examples, the common interface is the `Polygon` base class. The plugin
libraries provide concrete polygon types such as `Triangle`, `Square`, and
`Quadrilateral`. When a plugin library is loaded, it registers builder
functions into a shared factory, and the main program can then create the
requested polygon by name.

## The two subfolders

- `FactoryPlugin`
  Demonstrates automatic registration through functions marked with
  `__attribute__((constructor))`. Those functions are executed automatically
  when the shared library is loaded.

- `FactoryPluginII`
  Demonstrates automatic registration through namespace-scope static objects.
  When the shared library is loaded, the constructors of those objects register
  the builders in the shared factory.

So the two examples have the same overall goal and almost the same structure.
They differ mainly in the mechanism used to populate the factory when a plugin
library is loaded.

## Common structure of the examples

Both subfolders contain:

- a small polygon hierarchy with a base class and a few concrete derived
  classes;
- a shared factory object mapping names to builder functions;
- a file `plugins.txt` listing the shared libraries to load;
- a main program that loads the plugins and creates objects by querying the
  factory.

Both examples also rely on the `LoadLibraries` utility from
`Examples/src/LoadLibraries` to load the shared objects listed in `plugins.txt`.

## Prerequisite

Before building either example, make sure the dynamic-loading utility has been
built and installed:

```bash
cd Examples/src/LoadLibraries
make alllibs
make install
```

This installs `libloadlibs` into `Examples/lib`, where the plugin examples
expect to find it.

## What these examples teach

- the general purpose of a plugin architecture;
- how a polymorphic factory supports runtime extension of a program;
- how dynamically loaded shared libraries can register new concrete products;
- two different automatic-registration techniques in C++.
