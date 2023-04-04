# Another example of reference bindings #

This another example of reference bindings which illustrates
the mechanism at the base of move semantic. 

Remember that a rvalue reference can only bind to a rvalue, so to a
value that can be moved safely (or that you have indicated that can be
moved with `std::move`). It is then up to you to decide what to do for
the "moving".

# What do I get from this example? #
- A practical example of how reference bindings work, including rvalue references

Remember that the language provides move constructor and move assignment operator by default, so in most cases you have to do nothing and your class already implements move semantic. But there are rules  see 
[here](https://stackoverflow.com/questions/24342941/what-are-the-rules-for-automatic-generation-of-move-operations) and [here](https://images.app.goo.gl/hWVF7KGw46KnDTaq6).

