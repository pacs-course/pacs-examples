# GenericFactory

This folder contains a small generic implementation of the factory pattern.
The code is template-based and header-only: the main reusable components are
`Factory.hpp` and `Proxy.hpp`.

The example demonstrates two related use cases:

- an object factory, where identifiers are mapped to builders that create
  polymorphic objects;
- a function factory, where identifiers are mapped directly to callable objects.

## Files in the folder

- `Factory.hpp`
  Defines the class template `GenericFactory::Factory`, implemented as a
  singleton, plus the alias `FunctionFactory`.

- `Proxy.hpp`
  Defines `GenericFactory::Proxy`, a helper that automatically registers a
  concrete type into a factory.

- `main_testAbstractFactory.cpp`
  Contains a small interactive example that uses both an object factory and a
  function factory.

- `Makefile`
  Builds the example executable and provides the `install` target for the
  headers.

## What the code provides

### Object factory

The class template

```cpp
template <typename AbstractProduct, typename Identifier,
          typename Builder = std::function<std::unique_ptr<AbstractProduct>()>>
class Factory;
```

stores builders indexed by an identifier. Client code registers builders with
`add()` and later creates objects with `create()`.

This is useful when:

- the concrete type must be selected at runtime;
- the caller only knows the abstract base type;
- object construction must be decoupled from object use.

### Function factory

The alias

```cpp
template <typename Identifier, typename FunType>
using FunctionFactory = Factory<void, Identifier, FunType>;
```

reuses the same machinery to store callable objects instead of object builders.
In that case client code uses `get()` rather than `create()`.

### Proxy-based automatic registration

`Proxy.hpp` provides a helper that registers a concrete product in a factory as
soon as the proxy object is constructed. This is useful in plugin-like
architectures or whenever registration should happen automatically at static
initialization time.

## Compilation

From inside `Examples/src/GenericFactory`, build the example with:

```bash
make
```

or equivalently

```bash
make all
```

This compiles `main_testAbstractFactory.cpp` and produces the executable:

```text
main_testAbstractFactory
```

You can then run it with:

```bash
./main_testAbstractFactory
```

The program first demonstrates the object factory by asking for an identifier
and creating a corresponding derived object. It then demonstrates the function
factory by asking for a function identifier and two numeric arguments.

## Other Makefile targets

- `make clean`
  Removes the executable and object files.

- `make distclean`
  Performs a deeper cleanup and also removes dependency files.

- `make doc`
  Runs Doxygen using the common configuration of the repository.

- `make install`
  Copies `Factory.hpp` and `Proxy.hpp` into `Examples/include`.

Since the implementation is header-only, `install` is the way to make the
generic factory facilities available to other examples in the repository.

## What this example teaches

- how to implement a generic object factory with templates;
- how to use a singleton to centralize registration and lookup;
- how to reuse the same infrastructure as a function factory;
- how to automate registration through static proxy objects;
- how variadic templates and perfect forwarding support flexible builders.

For a more detailed explanation of `Factory.hpp`, see
`GenericFactory_explanation.md`.
