# `for_each` on a tuple #

A `std::tuple` is a heterogeneous container: each element may have a different type. This is very useful, but it has an immediate consequence: unlike a `std::vector`, a tuple cannot be traversed with an ordinary loop, because the compiler must know the type of each element at compile time.

So, if I want to "do something" to each component of a tuple, I need a different technique. This is exactly the purpose of the function

```cpp
apsc::for_each(tuple, f);
```

which applies a callable object `f` to all elements of the tuple, one after another.

For example,

```cpp
auto t = std::make_tuple(2, 3.5, true);
apsc::for_each(t, [](auto const &x) { std::cout << x << "\n"; });
```

prints all the tuple elements even if they have different types.

Why is this not immediate? Because a tuple is not indexed at run time in the same way as a vector. Access to tuple elements is done through `std::get<I>(tuple)`, where the index `I` is a compile-time constant. Therefore, to visit all elements we need a compile-time mechanism that generates the indices `0, 1, 2, ...`.

This example shows two implementations of the same idea.

## The C++17 version ##

In pre-C++20 code the implementation is based on `std::index_sequence`.

The public function

```cpp
template <class Tuple, class F>
constexpr decltype(auto) for_each(Tuple &&tuple, F &&f);
```

computes the tuple size and builds the compile-time sequence

```cpp
std::make_index_sequence<std::tuple_size_v<...>>{}
```

This sequence is then passed to an internal helper, `detail::for_each_impl`, which is only an implementation detail and should not be used directly.

Inside the helper, the pack of indices is expanded in

```cpp
(f(std::get<I>(tuple)), ...);
```

This is a fold expression over the comma operator. If the tuple has four elements, the compiler generates something equivalent to

```cpp
f(std::get<0>(tuple));
f(std::get<1>(tuple));
f(std::get<2>(tuple));
f(std::get<3>(tuple));
```

The important point is that the expansion is done at compile time.

## The C++20 version ##

With C++20 the same goal can be achieved more directly using `std::apply`.

`std::apply` takes a callable and a tuple, unpacks the tuple, and passes its elements as arguments to the callable. So the implementation becomes

```cpp
std::apply(
  [&f](auto &&...args) { (f(std::forward<decltype(args)>(args)), ...); },
  std::forward<Tuple>(tuple));
```

Again, the unpacked arguments are traversed with a fold expression. This version is shorter because `std::apply` hides the explicit index machinery.

## Why return `f`? ##

The function returns the callable after the traversal. This is useful when the callable carries state. For instance, one may use a functor that counts how many elements have been visited or accumulates some information while traversing the tuple.

In the test program there is a simple example of this idea:

```cpp
int sum = 0;
auto accumulate = [&sum](int x) { sum += x; };
apsc::for_each(t, accumulate);
```

After the call, `sum` contains the total of the integer elements.

## The test program ##

The file `main_for_each_tuple.cpp` contains a small test suite that exercises the public interface `apsc::for_each` on several cases:

- tuples of integers;
- tuples with mixed types;
- empty tuples;
- single-element tuples;
- stateful callables used for accumulation;
- strings, vectors, booleans, and function pointers.

The important design point is that the tests use `apsc::for_each`, not the internal helper. The helper is just there to implement the C++17 version.

## What do I learn from this example ##

- How to traverse a `std::tuple`, even though its elements may have different types;
- The role of compile-time integer sequences such as `std::index_sequence`;
- The use of fold expressions to apply an operation to a parameter pack;
- How `std::apply` simplifies tuple metaprogramming in modern C++;
- The distinction between a public interface and an implementation detail in template code.
