#Some utilities of the standard library#

Some code examples in this directory are modified version of those in
*The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
by Nicolai M. Josuttis, Addison-Wesley, 2012*

Some updates have been made to introduce some C++17 goodies

* `cont/` examples on the use of different containers

* `set/`  use of sets with different comparison operators

* `sort/` Use comparison operators to change sorting criteria.

* `SetEdge/` An esample of set of Edges that describe a graph where different comparison operators enable directed (Edges may be repeated if they have  different orientation) or undirected graphs.

* `tuple/` several examples of the use of tuples and tie with also
some example of structured bindings

* `Optional/` the new optional template class of the standard library,
that allows to mark a value as "unset value" in a safe way. Useful to
mark missing data!

* `Variant/` The variant template class (c++17) is a sophisticated and
type-safe C++ extension of a union. It allows to have variables that
may hold different types. Useful if you have to read from a file where
the content may be interpreted as belonging to different types.
It contins also an example of use of `std::visit()`.
