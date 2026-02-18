# A template class for derivatives of a function by finite differences #

This example uses template metaprogramming to implement derivatives of any order,
approximated with finite differences. It is also an example of recursion:
the N-th derivative of `f` is computed as the first derivative of the
`(N-1)`-th derivative, recursively, down to the base case.

To get a more stable (and accurate) formula for higher order derivatives I alternate backward and
forward differences, but you can also use centered formula. However, beware that the standard three point formula for the second derivative is obtained
by choosing the backward or forward alternative, not the centered ones. With the algorithm I have implemented a centered second order derivative will use a stencil of five points, not three.

The example also includes a recursive lambda-based implementation of centered
derivatives. The use of `if constexpr` closes recursion at compile time
without requiring a dedicated specialization.

**Note** The code in this folder may be used by other examples.
Run `make install` to copy headers to the expected include folder.

**What does this example show?**

- Recursion with templates.
- The `if constexpr` construct to select branches at compile time.
- C++20 concepts and constraints for safer template instantiation.
- Helper functions to simplify creation of derivative operators.
- A recursive lambda approach inspired by *Discovering Modern C++* (Peter Gottschling).
