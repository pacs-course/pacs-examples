# Niebloids

This example shows a very small custom niebloid.

The header [`norm2.hpp`](./norm2.hpp) defines `apsc::niebloids::norm2`, a
function object that computes the Euclidean norm, or 2-norm, of the values
stored in a range:

`\|x\|_2 = sqrt(x_1^2 + x_2^2 + ... + x_n^2)`

In the example, the code accepts any type satisfying the
`std::ranges::range` concept, so it can work with `std::vector`, `std::array`,
views, and other range-like objects.

## What is a niebloid?

In modern C++, many standard library algorithms in the ranges library are not
ordinary functions, but function object: objects of class type that define
`operator()`, so they can still be called with function-like syntax.

For example:

```cpp
std::ranges::sort(v);
```

`std::ranges::sort` is not a normal function. It is a function object, often
called a *niebloid*.

This style is useful because it allows the standard library to:

- group algorithms inside namespaces such as `std::ranges`;
- customize overload resolution in a controlled way;
- keep the syntax simple for the user.

In this example, `norm2_fn` is the function object type, while

```cpp
inline constexpr norm2_fn norm2{};
```

creates the actual object used in the calls.
It is declared inline to avoid the one definition rule and constexpr since 
is an immutable object (is not meant to be changed by the user in any way).

## Files

- [`norm2.hpp`](./norm2.hpp): defines the `norm2_fn` function object and the
  `norm2` niebloid object.
- [`main.cpp`](./main.cpp): builds a small vector, calls `niebloids::norm2`,
  prints the result, and checks that the norm of `{3,4}` is `5`.

## How it works

The call operator in `norm2_fn`:

1. accepts a generic range;
2. loops over all elements in the range;
3. accumulates the sum of the squares;
4. returns the square root of the sum.

The object `norm2` is declared `inline constexpr`, so it can live in a header
file and be included in several translation units safely.

## Build and run

From this directory:

```bash
make
./main
```
# What do I learn here?
- A technique very much used in the standard library.
- A use of the range concept.