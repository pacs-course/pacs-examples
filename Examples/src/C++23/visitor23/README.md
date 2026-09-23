# Two implementations of the Visitor pattern

This workspace implements the same example in two ways, one is a classical imolementation of
 the visitor pattarn, using polymorphism and double dispatch technique. The second uses the 
 varaint + visit tools introduced in C++17. Hoever, a difficulty here is that the calssses to be visit may be 
 composing classes (composer design pattern) and we need to dispatch the visitor to the componets. This is a situation where the *deducing this* technique intriduced in c++23 is quite handy.
 
IN the examplee, ach visitor definesan operation for wheels, a body, an engine, and the car itself. One visitor prints the elements; the other prints actions such as kicking a wheel or starting the
engine. Adding an operation means adding a visitor, without rewriting the element
classes.

| File | Purpose |
| --- | --- |
| `main_visit_classic.cpp` | Classic Visitor using inheritance and virtual functions. |
| `main_visit23.cpp` | A `std::variant` visitor built from lambdas, using C++23 deducing `this` for recursive traversal. |
| `Makefile` | Builds two separate executables, since each source defines `main()`. |

## Build and run

Use GNU Make and a compiler supporting C++23 explicit object parameters. The
Makefile defaults to `g++-14`; selecting C++23 mode alone is insufficient on an
older compiler that does not implement deducing `this`. 

```sh
make                         # Build both executables
make run                     # Run the classic example, then the C++23 example
make main_visit23            # Build just the C++23 example
make clean                   # Remove both executables
make CXX=clang++-20           # Select another compiler
```

The Makefile also accepts `CPPFLAGS`, `CXXFLAGS`, `LDFLAGS`, and `LDLIBS` overrides.
Run `make clean` before rebuilding with a different compiler or different flags.
Both examples use GCC diagnostic pragmas to suppress unused-parameter and
unused-variable warnings: several handlers only print a fixed message.

## Classic implementation: `main_visit_classic.cpp`

`CarElement` declares the virtual `accept(CarElementVisitor&)` function.
`Wheel`, `Body`, `Engine`, and `Car` derive from it. `CarElementVisitor` declares
four virtual `visit` overloads, and the two concrete visitor classes supply their
printing and action implementations.

The key expression inside each element's `accept` is `visitor.visit(*this)`.
For example, when visiting a wheel through a `CarElement*`:

1. Virtual dispatch selects `Wheel::accept` from the element's runtime type.
2. Inside that function, `*this` has the static type `const Wheel`, so overload
   resolution selects `visit(Wheel)`.
3. Virtual dispatch selects the implementation belonging to the concrete visitor.

This is double dispatch: the operation depends on both the element and the
visitor. The repeated `accept` overrides give overload resolution the concrete
element type. The classic `visit` overloads receive elements by value, so they
copy their arguments.

`Car::accept` first calls `accept` on its children, then visits the car itself.
Traversal therefore belongs to the element implementation in this version.

## C++23 implementation: `main_visit23.cpp`

The element classes no longer share a virtual base. Instead, `CarElement` is
`std::variant<Wheel, Body, Engine, Car>`: each value holds one of these alternatives.
`std::visit(visitor, element)` selects a handler for the currently held type at
runtime, while the compiler checks that the visitor can handle every alternative.

The helper

```cpp
template<class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
};
```

inherits from the supplied lambda closure types and brings their call operators
into one overload set. Each `overloaded{...}` object thus combines four handlers
into one visitor. These handlers take elements by const reference.

The `Car` handler calls `car.visitCarElements(self)` before printing its own
message. That member loops over the children and calls `std::visit` again.
Here the visitor decides when to traverse the children.

## Why deducing `this` matters here

Both car handlers have this form:

```cpp
[](this auto const& self, const Car& car) {
    car.visitCarElements(self);
    std::cout << "Visiting car" << '\n';
}
```

`self` is an explicit object parameter, supplied automatically by calling the
visitor. It is not an extra argument that `std::visit` must pass. Because the
lambda's call operator is a member of  `overloaded`, its object parameter deduces
the complete `overloaded` visitor type. Consequently, `self` is a const reference
to the visitor containing **all four handlers**, not merely the car lambda.
This use of derived-type deduction is described in the C++ proposal
[P0847R7, Recursive Lambdas](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0847r7.html#recursive-lambdas).

The call sequence is:

```text
std::visit(visitor, car)
  -> car handler, with self referring to the complete visitor
     -> car.visitCarElements(self)
        -> std::visit(self, each child)
           -> wheel, body, engine, or nested car handler
     -> print the car message
```

Passing only the car lambda would fail: it cannot handle wheels, bodies, and
engines, but `std::visit` requires a handler for every variant alternative.
Removing `this auto const& self` also removes the reference needed by the
existing traversal call. An ordinary lambda has no implicit `this` pointing to
its own closure; capturing `[this]` refers to an enclosing class object instead.

Deducing `this` therefore enables recursion through the complete overload set in
this implementation. It is not a requirement of the Visitor pattern in general:
the classic example works without it. A named function object with four
`operator()` overloads could likewise pass `*this` to `visitCarElements` without
using an explicit object parameter.

## Shared behavior and design tradeoffs

Each program first visits the engine separately, then the entire car, for each
visitor. Both should print the following, with additional blank lines around the
two groups:

```text
Visiting engine
Visiting front left wheel
Visiting front right wheel
Visiting back left wheel
Visiting back right wheel
Visiting body
Visiting engine
Visiting car

Starting my engine
Kicking my front left wheel
Kicking my front right wheel
Kicking my back left wheel
Kicking my back right wheel
Moving my body
Starting my engine
Starting my car
```

In both examples, `Car` stores non-owning pointers to elements created in `main`.
Those elements outlive traversal; the car does not delete them. Nested cars are
possible, but cycles would cause unbounded recursion.

The classic design expresses the element protocol through virtual interfaces;
the variant design lists the supported element types explicitly. Adding a new
element type requires updating the visitor interface and implementations in the
classic version, or the variant alternatives and handlers in the C++23 version.
Both make adding new operations straightforward by introducing another visitor.

The C++23 version hs the advantage of avoiding the need of polymorphic objects, but 
it add the *deducing this* complexity. 

# What do I learn here? #

- A nice and useful implementation of the C++23 *deducing this*.
- A revision of the visitor design pattern. A useful programming design.

