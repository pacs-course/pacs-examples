# Constexpr #

A simple example of a constexpr function and the difference with a
normal function.

Run `make` and then `examine.sh` to see that the normal function `cube`
creates a symbol, so the corresponding machine code that calls the
function has a jump-and-return to the function address, while
cubeconst is always resolved inline. Indeed it does not appear when
running nm.

Then the two versions that uses a constexpr function are compared in
size. You may note that the one that simply inlines the function is
larger. The one that resolve the value statically is smaller since the
value returned by `cubeconst` is computed directly at compile-time.

Finally, in `HornerConstexpr.hpp` you have an implementation of the
Horner algorithm for computing polynomials that thanks to template
programming, use of automatic type deduction and constexpr is able to
operate with real and complex floating points, and resolves the
computation statically if the arguments are constant expressions.
