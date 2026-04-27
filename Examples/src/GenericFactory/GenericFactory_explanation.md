# `GenericFactory`: detailed explanation

This folder contains a compact, reusable implementation of the factory pattern
written with C++ templates. The central file is `Factory.hpp`, which provides a
generic registry mapping identifiers to builders. The same template can be used
both as:

- an object factory, producing objects from a polymorphic hierarchy;
- a function factory, returning callable objects associated with an identifier.

The implementation is header-only and designed for didactic use, but it is
general enough to support several examples in the repository.

## The core idea

The factory pattern decouples object selection from object use.

Instead of writing code such as

```cpp
if(id == 1) return std::make_unique<Derived1>();
if(id == 2) return std::make_unique<Derived2>();
if(id == 3) return std::make_unique<Derived3>();
```

the code registers builders once and then creates the requested object by
looking up its identifier in a container.

In this implementation, the container is an `std::unordered_map` that stores
associations of the form

```cpp
identifier -> builder
```

where the builder is typically a function, lambda, or function object.

## Structure of `Factory.hpp`

The main template is:

```cpp
template <typename AbstractProduct, typename Identifier,
          typename Builder = std::function<std::unique_ptr<AbstractProduct>()>>
class Factory;
```

The three template parameters determine the behavior of the factory.

### `AbstractProduct`

This is the base type of the objects created by the factory.

Typical example:

```cpp
class Abstract
{
public:
  virtual void whoAmI() = 0;
  virtual ~Abstract() = default;
};
```

If the factory is used as an object factory, the builders will produce concrete
types derived from `AbstractProduct`, and the returned type will be a
`std::unique_ptr<AbstractProduct>`.

If the factory is used as a function factory, `AbstractProduct` is set to
`void`, and `create()` is no longer the intended interface.

### `Identifier`

This is the key type used to select a registered builder.

Examples:

- `int`
- `std::string`
- an enum type

The implementation stores identifiers in an `std::unordered_map`, so the type
must be hashable. The code also builds error messages through
`std::stringstream`, so the identifier should be streamable as well.

### `Builder`

This is the callable type stored in the factory.

In the default case it is:

```cpp
std::function<std::unique_ptr<AbstractProduct>()>
```

so each registered builder is a nullary callable returning a smart pointer to
the abstract base type.

However, the builder type is fully customizable. It may:

- accept arguments;
- be a function pointer;
- be a lambda wrapped in `std::function`;
- be any callable type compatible with the intended usage.

## Public interface of `Factory`

### `Instance()`

```cpp
static Factory &Instance();
```

The factory is implemented as a Meyers singleton:

```cpp
static Factory theFactory;
```

inside `Instance()`.

This means:

- the factory object is created on first use;
- there is one logical factory instance per template specialization;
- all users of the same specialization share the same registry.

For example:

```cpp
using MyFactory = GenericFactory::Factory<Abstract, int>;
auto &factory = MyFactory::Instance();
```

Every call to `MyFactory::Instance()` returns the same object.

### `add()`

```cpp
void add(Identifier const &, Builder_type const &);
```

This method registers a builder under a given identifier.

Example:

```cpp
factory.add(1, [] { return std::make_unique<Derived1>(); });
```

If the identifier is already present, the method throws
`std::invalid_argument`. Duplicate registration is therefore treated as an
error rather than silently overwriting an existing rule.

### `get()`

```cpp
Builder get(Identifier const &name) const;
```

This method retrieves the builder associated with the given identifier.

If the identifier is not present, it throws `std::invalid_argument`.

This method is especially useful when the factory is used as a function
factory, because in that case the stored object is already the final callable.

Example:

```cpp
FunType f = ffactory.get(2);
auto value = f(x, y);
```

### `create()`

```cpp
template <typename... Args>
std::unique_ptr<AbstractProduct> create(Identifier const &name,
                                        Args &&...args) const;
```

This is the object-oriented interface of the factory.

It:

1. finds the builder associated with `name`;
2. forwards any runtime arguments to that builder;
3. returns the newly created object.

The call uses perfect forwarding:

```cpp
return this->get(name)(std::forward<Args>(args)...);
```

so builders are free to accept constructor-like parameters when needed.

Example:

```cpp
auto object = factory.create(1);
```

The implementation contains a `static_assert` preventing the misuse of
`create()` with `FunctionFactory`, where `AbstractProduct = void`.

### `registered()`

```cpp
std::vector<Identifier> registered() const;
```

This method returns the list of all currently registered identifiers.

It is useful for:

- diagnostics;
- menus in interactive programs;
- runtime discovery of available products or functions.

Since the internal container is an `unordered_map`, the order of the returned
identifiers is not guaranteed.

### `unregister()`, `clear()`, `empty()`

These are small utility methods for factory management:

- `unregister(name)` removes one entry;
- `clear()` removes all entries;
- `empty()` tests whether the registry is empty.

They are particularly useful in examples that dynamically register and later
remove plugin-provided entities.

## Internal storage

The internal registry is:

```cpp
using Container_type = std::unordered_map<Identifier, Builder_type>;
```

So the factory stores no objects directly. It stores only the rules needed to
produce them, or the callables to retrieve them.

This is an important design choice:

- registration is cheap;
- objects are created only on demand;
- the same identifier can be used repeatedly to produce multiple objects.

## `FunctionFactory`

The file defines the alias:

```cpp
template <typename Identifier, typename FunType>
using FunctionFactory = Factory<void, Identifier, FunType>;
```

This is a simple but effective reuse of the main template.

Instead of interpreting the stored value as a builder of objects, the code
stores a callable directly. For example:

```cpp
using FunType = std::function<double(double, double)>;
using FunFactory = GenericFactory::FunctionFactory<int, FunType>;
```

Then:

```cpp
auto &ffactory = FunFactory::Instance();
ffactory.add(1, [](double a, double b) { return a * b; });
FunType f = ffactory.get(1);
```

No second factory class is needed: the same template covers both use cases.

## How `main_testAbstractFactory.cpp` uses the factory

The example file in this folder demonstrates both factory styles.

### Object factory part

The code defines:

- an abstract base class `Abstract`;
- three concrete derived classes `Derived1`, `Derived2`, `Derived3`;
- a type alias

```cpp
using MyObjectFactory = GenericFactory::Factory<Abstract, int, Builder>;
```

where

```cpp
using Builder = std::function<std::unique_ptr<Abstract>()>;
```

The functions `build1`, `build2`, and `build3` are the concrete builders.

The function `loadFactory()` explicitly registers the first two:

```cpp
factory.add(1, build1);
factory.add(2, build2);
```

The third is registered through a proxy:

```cpp
namespace
{
GenericFactory::Proxy<MyObjectFactory, Derived3> P3{3, build3};
}
```

So the example shows both manual registration and automatic registration.

At runtime, the program:

1. gets the singleton factory;
2. asks for the registered identifiers;
3. prompts the user to choose one;
4. creates the requested object through `factory.create(i)`;
5. calls `whoAmI()` on the created object.

### Function factory part

The example also defines:

```cpp
using FunType = std::function<double(const double &, const double &)>;
using FunFactory = GenericFactory::FunctionFactory<int, FunType>;
```

Then `loadFunctions()` registers three callables:

- multiplication;
- `std::ellint_1`;
- `std::atan2`.

The program retrieves the chosen callable with `get()` and then evaluates it on
user-provided inputs.

This demonstrates that `Factory` is not restricted to polymorphic object
creation. It can also act as a named callable registry.

## Role of `Proxy.hpp`

`Proxy.hpp` provides the helper:

```cpp
template <typename Factory, typename ConcreteProduct>
class Proxy;
```

Its purpose is to bind registration to construction of a small helper object.

The default constructor overload:

```cpp
Proxy(Identifier_type const &name);
```

registers the static member `Build()` into the target factory.

`Build()` simply constructs a `ConcreteProduct` and wraps it in the expected
result type, typically `std::unique_ptr<AbstractProduct>`.

The second overload:

```cpp
Proxy(Identifier_type const &name, Builder_type const &b);
```

registers a caller-supplied builder instead.

This is useful when:

- the factory builder has a custom signature;
- construction requires extra logic;
- the default `Build()` helper is not the desired builder.

The common idiom is to place proxy objects at namespace scope:

```cpp
namespace
{
GenericFactory::Proxy<MyFactory, Derived> p{"derived"};
}
```

Then registration happens automatically during static initialization.

## Strengths of this implementation

- It is generic and compact.
- It cleanly separates registration from use.
- It supports both object factories and function factories.
- It works well with plugin-style architectures.
- It uses `std::unique_ptr`, so ownership of created objects is explicit.
- It supports builders with arguments through variadic templates and perfect
  forwarding.

## Important assumptions and limitations

- The factory is a singleton, so all code using the same specialization shares
  the same global registry state.
- The identifier type must work with `std::unordered_map` and should also be
  printable for diagnostics.
- Registration and lookup are not synchronized, so concurrent modification
  would require extra protection.
- The default `Proxy::Build()` assumes a pointer-based result type compatible
  with `new ConcreteProduct()`.
- Because registration is global for each specialization, examples should clear
  the factory if they need a clean state between runs in the same process.

## Summary

`Factory.hpp` implements a reusable generic registry of builders keyed by an
identifier. The same template supports runtime creation of polymorphic objects
and retrieval of named callable objects. `Proxy.hpp` complements it by making
automatic registration straightforward.

Together, these files provide a simple but powerful infrastructure for examples
that need runtime selection, plugin-style extension, or configurable creation
of objects and functions.
