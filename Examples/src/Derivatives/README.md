#A template class for derivatives of a function by finite differences#

To get a more stable (and accurate) formula I alternate backward and
forward differences. It shows also some use of `std::is_same<>` and of the `if constexpr` construct.

We also show how to make helper functions to create specific
instances. An alternative would be use the new class template argument
deduction rule introcuced in C++17, but here is more complex since we
are using a template value (enum) to select the differencing type. I think
that helper `make`-style functions are much simpler (at least in this case).

Derived from a code in *"Discovering Modern C++", P. Gottschiling,
Addison Wesley, 2016.*
