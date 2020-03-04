#Computing greater common divisor of two integers at compile time#

I present several techniques to compute the reater common divisor of two given integers, letting the compiler to the computation at compile time. Some are working also with C++ verions earlier than C++11.

**Note** C++17 has introduced two function templates, `std::gcd<M,N>()$ and `std::lcm<N,M>()` to compute the greater common divisor and least common multiple of two integers. So this code is only fhere to give an example of numeric metaprogramming, If you use C++ version at least 17, you'd better use the tools provided by the standard library.

**A second note** If you do `nm --demangle main_gcd` to examine the symbols in the produced executable, you may discover
that none of the functions/classes defined in `gcd.hpp` appear! All has been resolved at compile time, no function call! (you may need to compile with optimization on).