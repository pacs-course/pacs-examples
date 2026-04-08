# CloningUtilities.hpp - C++20 Cloning Wrapper Overview

This document describes the current implementation of `CloningUtilities.hpp`.
The header provides a clone-aware smart wrapper for polymorphic composition with value semantics.

## Why This Utility Exists

Using raw pointers for polymorphic aggregation can produce dangling references and unclear ownership.
`PointerWrapper<T>` solves this by:

- owning resources with `std::unique_ptr<T>`
- preserving deep-copy semantics through `clone()`
- supporting polymorphic base/derived conversions when valid
- exposing a pointer-like API (`*`, `->`, `get`, `reset`, `release`, `swap`)

## Clone Detection in `apsc::TypeTraits`

### `has_clone<T>`

The trait is implemented with a primary template and a `std::void_t` specialization:

- primary template: `false`
- specialization: enabled when `decltype(std::declval<const T&>().clone())` is valid
- specialization result: `true` only if `std::unique_ptr<T>` is convertible to the clone return type

So a type is considered clonable only when both conditions hold:

1. `clone()` is callable on a const object
2. `std::unique_ptr<T>` is implicitly convertible to the clone return type

The direction of the convertibility check is intentional: it accepts `Derived::clone()` returning
`std::unique_ptr<Base>`, because `std::unique_ptr<Derived>` is convertible to `std::unique_ptr<Base>`
via `unique_ptr`'s converting constructor (whenever `Derived*` is convertible to `Base*`).

### Convenience interfaces

- `isClonable<T>()`
- `has_clone_v<T>`
- concept `Clonable`

`PointerWrapper` uses the concept directly:

```cpp
template <TypeTraits::Clonable T>
class PointerWrapper;
```

## `PointerWrapper<T>` Design

`PointerWrapper<T>` is a value-like owner for polymorphic objects.
Internally it stores:

```cpp
using Ptr_t = std::unique_ptr<T>;
```

It also re-exports `unique_ptr` member types:

- `pointer`
- `element_type`
- `deleter_type`

## Construction and Ownership

Supported constructors:

- default constructor (null wrapper)
- from `const T&` (clone-based construction)
- from `Ptr_t&&` (move a unique pointer)
- from raw `T*` (explicit ownership transfer)
- copy constructor (deep copy by clone)
- move constructor (default)

## Cross-Type Conversions (Derived -> Base)

Conversion constructor and assignments are constrained with C++20 `requires`:

```cpp
requires std::is_convertible_v<U*, T*>
```

This applies to:

- copy conversion constructor
- copy conversion assignment
- move conversion constructor
- move conversion assignment

Invalid conversions are rejected at overload resolution time (no static_assert-based diagnostics are used here anymore).

## Assignment Behavior

### Copy assignment (`PointerWrapper<T> <- PointerWrapper<T>`)

- handles self-assignment
- clones source when non-null
- stores null when source is null

### Copy assignment from `Ptr_t const&`

- clones pointed object when non-null
- sets null otherwise

### Move assignments

- same-type move assignment is defaulted
- converting move assignment transfers ownership via `release()`

## Pointer-Like Interface

Main pointer API:

- `operator*` / `operator->` (const and non-const)
- `pointer get() const noexcept`
- `pointer release() noexcept`
- `void reset(pointer = nullptr) noexcept`
- `void swap(PointerWrapper&) noexcept`
- `deleter_type& get_deleter() noexcept`
- `deleter_type const& get_deleter() const noexcept`
- `explicit operator bool() const noexcept`

## Factory Helper

`make_PointerWrapper<B, D>(Args&&...)` constructs `D` and returns `PointerWrapper<B>`.

It is constrained by:

```cpp
requires std::derived_from<D, B> && std::constructible_from<D, Args...>
```

This enforces both hierarchy correctness and constructor availability.

## Comparisons

### Pre-C++20 branch

When compiling with language mode earlier than C++20, classic relational operators are provided (`<`, `<=`, `>`, `>=`, `==`, `!=`) comparing raw stored pointers.

### C++20 branch

For C++20 and later:

- wrapper-wrapper comparison is implemented via `operator<=>`
- wrapper-wrapper equality is provided explicitly with `operator==`
- wrapper-nullptr comparison uses constrained `operator<=>`
- wrapper-nullptr equality uses explicit `operator==`

The C++20 comparison overloads are marked `noexcept`.

## Hash Support

`std::hash<apsc::PointerWrapper<T>>` is specialized so wrappers can be used in unordered containers.
Hashing is delegated to the hash of the underlying pointer returned by `get()`.

## Minimal Usage Example

```cpp
struct Base {
  virtual ~Base() = default;
  virtual std::unique_ptr<Base> clone() const = 0;
};

struct Derived : Base {
  int v{};
  explicit Derived(int x) : v(x) {}
  std::unique_ptr<Base> clone() const override {
    return std::make_unique<Derived>(*this);
  }
};

apsc::PointerWrapper<Base> a = apsc::make_PointerWrapper<Base, Derived>(42);
apsc::PointerWrapper<Base> b = a;  // deep copy via clone
if (b) {
  // use b as a pointer-like object
}
```

## Notes

- The utility assumes `clone()` is semantically a deep copy operation.
- The wrapper models ownership like `std::unique_ptr`, but with explicit copy semantics implemented through cloning.
- Correct polymorphic deletion still requires a virtual destructor in the base class.
