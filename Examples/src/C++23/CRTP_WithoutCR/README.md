# Delegating This in C++23

This workspace is a compact demonstration of the C++23 `deducing this` feature, also known as the "delegating this" idiom. The program shows how a base-class member function can be written once and then work with the actual derived object type, without requiring virtual dispatch or a CRTP base class.

The source file is [main.cpp](main.cpp). It builds with the toolchain in [Makefile](Makefile).

## What this example is doing

The program defines a `Base` type with an `interface()` member and an overloaded `operator[]`:

```cpp
struct Base {
  template <typename Self>
  void interface(this Self&& self) {
    self.implementation();
  }

  int& operator[](this auto& self, std::size_t index) {
    return self.data[index];
  }
};
```

The important part is the signature:

```cpp
template <typename Self>
void interface(this Self&& self)
```

This means the function is not tied to a fixed `this` type. Instead, the compiler deduces the actual object type of the call site. `self` may be a `Base`, `Derived1`, `Derived2`, or even a loose reference type, and the call behaves as if it were made on the concrete object.

The derived classes override `implementation()`, so the output depends on the dynamic identity of the object as seen at the call site.

## Why it matters

The core idea is simple:

- ordinary member functions are bound to a specific object type in the usual way
- `deducing this` lets a member function deduce the type of the receiver from the expression used to call it
- this enables a single function to be reused by many related types without the usual base-class virtual machinery

In other words, the method can behave as a generic member function over the receiver type.

## Relationship to CRTP

The pattern is often compared to the Curiously Recurring Template Pattern (CRTP), because both aim to let a base interface operate on the actual derived type.

With CRTP, the pattern looks roughly like this:

```cpp
template <class Derived>
struct Base {
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
};
```

This works because the base class is parameterized by the derived class, and the cast exposes the concrete type.

The `deducing this` idiom achieves a similar goal without making the class hierarchy template-based:

```cpp
struct Base {
  template <typename Self>
  void interface(this Self&& self) {
    self.implementation();
  }
};
```

The difference is important:

- CRTP requires a template base and a statically known inheritance relationship
- `deducing this` works with ordinary member functions and ordinary inheritance
- it can express behavior that is more like a generic member function over the receiver than a CRTP-style static cast

In practice, `deducing this` often supersedes CRTP in cases where the goal is not "inject a templated base into every derived type" but rather "write one member function that adapts to whichever type it was called on".

## Why it may supersede CRTP in some designs

CRTP is excellent when you want a reusable base that depends on the exact derived type. But it has costs:

- the class hierarchy becomes templated and more specialized
- the design is tightly coupled to static type relationships
- users must often understand the inheritance template pattern to make sense of the code

`deducing this` can be a cleaner fit when:

- the API is naturally expressed as a member function on a common base
- the behavior differs by derived type but the hierarchy remains ordinary
- you want a single implementation with no manual static casts
- the code is conceptually a generic method over the receiver rather than a template-heavy base class

So while CRTP is still powerful, the delegating-this idiom can replace it in a subset of the same problems: small, reusable, receiver-aware APIs that want the concrete object type without the CRTP boilerplate.

## What the sample prints

The program creates a base object and a few derived objects with different `data` contents:

- `Base` fills no values and prints `Implementation Base`
- `Derived1` fills with `1`
- `Derived2` fills with `2`
- `Derived3` fills with `3`

The `execute()` templates call `operator[]`, and the `execute2()` templates call `interface()`. Because `interface()` is written using `deducing this`, the `implementation()` call resolves to the derived object's override when the call is made through a derived object.

The output therefore demonstrates the receiver-aware dispatch:

```text
Base
Derived1
Derived2
Derived3
```

with the actual `implementation()` selected by the concrete object type.

## Caveats

This example is intentionally minimal. There are a few important cautions:

- `Base::data` is zero-initialized in the sample to avoid reading uninitialized values
- `operator[]` overloads are chosen by the cv-qualification and reference category of the call
- `deducing this` is a language feature, not a magic replacement for all polymorphic patterns

It is best understood as a way to make a base member function adapt to the concrete receiver while preserving a normal class hierarchy.

## Summary

This workspace is a focused example of the C++23 `deducing this` idiom. It shows that a base member can recover its concrete receiver type without CRTP, and it illustrates why this technique can be a cleaner alternative in some designs that previously relied on CRTP-style static dispatch.
