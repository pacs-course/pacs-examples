# Another example of reference bindings #

This is just another example of reference bindings which illustrates
the mechanism at the base of move semantic. 

Remember that a rvalue reference can only bind to a rvalue, so to a
value that can be moved safely (or that you have indicated that can be
moved with `std::move`). It is then up to you to decide what to do for
the "moving".
