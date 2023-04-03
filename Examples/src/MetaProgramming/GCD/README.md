# Computing greater common divisor of two integers at compile time #

I present two techniques based on recursion to compute the greater common divisor of two given integers, letting the compiler to the computation at compile time.

**Note** C++17 has introduced two function templates, `std::gcd<M,N>()` and `std::lcm<N,M>()` to compute the greater common divisor and least common multiple of two integers. So this code is only fhere to give an example of numeric metaprogramming, If you use C++ version at least 17, you'd better use the tools provided by the standard library.

**A second note** If you compile with `DEBUG=no` and do `nm --demangle main_gcd` to examine the symbols in the produced executable, you may discover
that none of the functions/classes defined in `gcd.hpp` appear! All has been resolved with inlining! And in the case of constant expression the compiler may have
computed the value directly

# What I learn from this example #
- A simple use of recursion with templates. Using two different strategies. 