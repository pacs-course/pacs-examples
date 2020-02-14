# Constexpr #

A simple example of a constexpr function and the difference with a
normal function.

Run ``examine.sh`` to see that the normal function creates a symbol,
so the corresponding machine code that calls the function has a
jump-and-return to the function address, while cubeconst is always
resolved inline. Indeed it does not appear when running nm.

