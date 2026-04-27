# `Proxy.hpp`: detailed explanation

This file explains the helper class defined in `Proxy.hpp`:

```cpp
template <typename Factory, typename ConcreteProduct>
class Proxy;
```

The role of this class is simple: it performs registration of a concrete
product into a generic factory when a `Proxy` object is constructed.

This is useful when you want registration to happen automatically, without
writing an explicit call such as

```cpp
factory.add(id, builder);
```

in the main program.

## Why the proxy exists

The factory class in `Factory.hpp` already provides the mechanism for storing
builders associated with identifiers. However, client code still needs to call
`add()` somewhere.

The proxy wraps that operation inside a small object whose constructor performs
the registration. This makes it possible to write code like:

```cpp
namespace
{
GenericFactory::Proxy<MyFactory, Derived3> P3{3, build3};
}
```

and obtain automatic registration when `P3` is constructed.

This pattern is especially useful in:

- plugin-like architectures;
- static self-registration mechanisms;
- code where each concrete type should register itself close to its own
  definition.

## Template parameters

The proxy has two template parameters.

### `Factory`

This is the target factory type.

It is expected to be a specialization of `GenericFactory::Factory`, for
example:

```cpp
using MyFactory = GenericFactory::Factory<Abstract, int, Builder>;
```

The proxy uses this type to recover:

- the abstract product type;
- the identifier type;
- the builder type;
- the singleton instance returned by `Factory::Instance()`.

### `ConcreteProduct`

This is the concrete type to be associated with the factory registration.

It is usually a class derived from the factory abstract product type, for
example:

```cpp
class Derived3 : public Abstract
{
  // ...
};
```

## Public type aliases

`Proxy` exposes several aliases derived from the factory type:

```cpp
using AbstractProduct_type = typename Factory::AbstractProduct_type;
using Identifier_type = typename Factory::Identifier_type;
using Builder_type = typename Factory::Builder_type;
using Result_type = std::invoke_result_t<Builder_type>;
using Factory_type = Factory;
```

These aliases make the code easier to write and tie the proxy to the
corresponding factory traits.

The most important one is:

```cpp
using Result_type = std::invoke_result_t<Builder_type>;
```

which represents the return type of the builder. In the standard object-factory
case, this is typically:

```cpp
std::unique_ptr<AbstractProduct>
```

## Constructors

The class provides two registration mechanisms.

### 1. Registration through the built-in builder

```cpp
Proxy(Identifier_type const &name);
```

This constructor registers the static member function `Build()` under the
identifier `name`.

Internally, it does:

```cpp
Factory_type &factory(Factory_type::Instance());
factory.add(name, &Proxy<F, C>::Build);
```

So the steps are:

1. access the singleton factory;
2. use `Build()` as the builder;
3. register that builder under the chosen identifier.

This is the most convenient version when default construction of the concrete
type is enough.

### 2. Registration through a user-supplied builder

```cpp
Proxy(Identifier_type const &name, Builder_type const &b);
```

This constructor registers the builder `b` directly.

Internally, it does:

```cpp
Factory_type &factory(Factory_type::Instance());
factory.add(name, b);
```

This version is useful when:

- construction requires custom logic;
- the builder needs to be a lambda or function object;
- the default `Build()` member is not appropriate;
- the builder signature is more specialized than simple default construction.

This is exactly the version used in `main_testAbstractFactory.cpp` for:

```cpp
GenericFactory::Proxy<MyObjectFactory, Derived3> P3{3, build3};
```

## The static `Build()` member

The class defines:

```cpp
static Result_type Build()
{
  return Result_type(new ConcreteProduct());
}
```

This is the default builder provided by the proxy.

Its purpose is to create an instance of `ConcreteProduct` and wrap it in the
result type expected by the factory builder.

In the standard case, if `Result_type` is
`std::unique_ptr<AbstractProduct>`, then this is equivalent in spirit to:

```cpp
return std::make_unique<ConcreteProduct>();
```

although the code uses direct construction from `new ConcreteProduct()`.

This implies an important assumption: `Result_type` must be constructible from
a raw pointer to `ConcreteProduct`.

That assumption holds for the default object-factory setup, but it may not hold
for arbitrary builder types. In those cases, the second constructor overload,
which accepts an explicit builder, is the appropriate choice.

## Copy control

The proxy is intentionally non-copyable:

```cpp
Proxy(Proxy const &) = delete;
Proxy &operator=(Proxy const &) = delete;
```

This is sensible because the object is not meant to represent reusable state.
Its purpose is simply to trigger registration once.

## Typical usage pattern

The most common way to use `Proxy` is to define a namespace-scope static object.

Example:

```cpp
namespace
{
GenericFactory::Proxy<MyFactory, Derived1> p1{"d1"};
GenericFactory::Proxy<MyFactory, Derived2> p2{"d2"};
}
```

When the translation unit is initialized, `p1` and `p2` are constructed, and
their constructors register the corresponding builders automatically.

This means that later code can simply do:

```cpp
auto &factory = MyFactory::Instance();
auto obj = factory.create("d1");
```

without explicitly calling `add()`.

## Use in the example program

In `main_testAbstractFactory.cpp`, the proxy is used like this:

```cpp
namespace
{
GenericFactory::Proxy<MyObjectFactory, Derived3> P3{3, build3};
}
```

This means:

- the object factory type is `MyObjectFactory`;
- the concrete registered type is `Derived3`;
- the identifier is `3`;
- the registered builder is `build3`.

So when the program later asks the factory to create object `3`, the registered
builder produces a `Derived3` instance.

This is a useful demonstration because the same program also registers other
builders explicitly through `factory.add()`. It therefore shows both possible
styles:

- manual registration;
- registration through a proxy object.

## Strengths of the proxy approach

- It keeps registration code close to the concrete type.
- It reduces boilerplate in the main program.
- It supports self-registration patterns.
- It works well with dynamically loaded modules and plugin-like systems.

## Important caveats

- Registration depends on construction of the proxy object, so object lifetime
  and initialization order matter.
- Because the target factory is a singleton, registration modifies shared
  global state for that factory specialization.
- The default `Build()` function assumes default constructibility of
  `ConcreteProduct` and pointer-based construction of `Result_type`.
- The current implementation is not synchronized, so concurrent registration
  would require external protection.

## Summary

`Proxy.hpp` provides a small helper that turns factory registration into an
object-construction side effect. It complements `Factory.hpp` by making
automatic registration straightforward, especially in examples where concrete
types should announce themselves without explicit setup code in `main()`.
