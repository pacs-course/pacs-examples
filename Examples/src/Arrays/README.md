# An example of the fixed-size array class#

``std::array<T,N>`` is a very convenient wrapper around C-style arrays, which makes the use of the latter obsolete in modern C++ programming.

 Standard arrays are aggregates, so you have all tools available for aggregates. At the same time they are also containers of the standard library, so you have
 iterators, you can use range based for-loop, you can use them with standard algorithms.

 They can also interface them easily with C-style fixed type arrays 
 (just in passing: also vectors can interface C-style dynamic arrays, using the same method `data()`).
 
# What do I learn here? #
 
 - A few features of `std::array'.
 - How to use `std::array` in a generic way.
 