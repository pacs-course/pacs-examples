# C++23 examples

This directory contains teaching examples of C++23 language and library
features: explicit object parameters (commonly called **deducing this**),
`std::expected`, and `std::mdspan`. The descriptions below summarize the README
in each subfolder; follow the links for detailed explanations and build instructions.

## Subfolders

### [CRTP_WithoutCR](CRTP_WithoutCR/README.md)

Demonstrates how explicit object parameters let a member of an ordinary base
class operate on the concrete receiver type without virtual dispatch or a
Curiously Recurring Template Pattern (CRTP) base. The example uses a shared
`interface()` and indexing operators with several derived classes. It illustrates
static dispatch through the type deduced at the call site and explains when
deducing this can simplify a design traditionally implemented with CRTP.

### [ExpressionTemplateWithoutCRPT](ExpressionTemplateWithoutCRPT/README.md)

Implements lazy, elementwise arithmetic on vectors of `double` in namespace
`ET23`. Expression nodes represent addition, subtraction, multiplication,
negation, exponential, and logarithm operations; constructing or assigning an
owning vector evaluates the expression without intermediate result vectors.
The tutorial covers scalar operands, storage access, operand lifetimes, and
the difference between retaining an expression and materializing its values.

The example explores replacing a CRTP interface with a non-template `Expr` base
and explicit object parameters. Its current arithmetic path calls concrete
members directly; the protected forwarding members are demonstrated separately.
COmpare with the code in `ExpressionTemplate` folder.

The main tutorial contains 50 checks, with an additional forwarding regression
test in `tests/forwarding.cpp`.

### [expected](expected/README.md)

Introduces `std::expected<T, E>` as a return type holding either a successful
result or error information. A string-to-integer conversion returns
`std::expected<int, std::string>` and uses `std::unexpected` to report conversion
errors. The caller demonstrates checking the result, reading `value()` or
`error()`, and supplying a fallback with `value_or()`. The example also explains
that `std::expected` does not catch exceptions automatically.

### [mdspan](mdspan/README.md)

Demonstrates `std::mdspan` as a non-owning multidimensional view over data stored
in `std::vector`. The program presents two-dimensional row-major and column-major
views, compile-time and runtime extents, and a three-dimensional view. It shows
how layouts map multidimensional indices to underlying storage and explains
the lifetime and bounds requirements of a view that does not own its elements.

### [visitor23](visitor23/README.md)

Compares two implementations of the Visitor pattern using a car composed of
wheels, a body, and an engine. One uses inheritance, virtual functions, and
double dispatch; the other combines `std::variant`, `std::visit`, and overloaded
lambdas. Both provide visitors that print the elements or perform illustrative
actions on them.

The C++23 version uses deducing this to give the car handler access to the
complete visitor, enabling recursive traversal of the car's components. The
example highlights how traversal responsibility and the handling of new element
types differ between the two designs, and builds two separate executables for
comparison. Compare with `DesignPattern/VisitorAndVariants`.

## Building the examples

Each subfolder provides its own `Makefile` and README with build and run
instructions. The explicit-object-parameter examples require a compiler that
implements deducing this. The `expected` and `mdspan` examples also require
support for their respective headers in the selected standard library;
enabling C++23 language mode alone is not sufficient. Consult the individual
READMEs for the toolchain settings used by each example.
