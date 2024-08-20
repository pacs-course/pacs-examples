# Some utilities of the standard library #

Some code examples in this directory are modified version of those in
*The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
by Nicolai M. Josuttis, Addison-Wesley, 2012*

Some updates have been made to introduce some C++17 goodies

* `cont/` examples on the use of different containers 

* `set/`  use of sets with different comparison operators

* `sort/` Use comparison operators to change sorting criteria. We also show the new sort constrained algorithm.

* `SetEdge/` An esample of set of Edges that describe a graph where different comparison operators enable directed (Edges may be repeated if they have  different orientation) or undirected graphs.

* `tuple/` several examples of the use of tuples and tie with also
some example of structured bindings

* `Optional/` the new optional template class of the standard library,
that allows to mark a value as *unset value* in a safe way. *Useful to mark missing data!*

* `Variant/` The variant template class (c++17) is a sophisticated and
type-safe C++ extension of a union. It allows to have variables that
may hold different types. Useful if you have to read from a file where
the content may be interpreted as belonging to different types.
It contins also an example of use of `std::visit()`.

* `Filesystem/` Utilities to examine the file system.

* `Reduce/` Two new stl parallel algorithms
  
* `RangesAndViews` An example of use of the new ranges and views of the standard library.

* `SortAndPermute` An useful utility if you need to sort a vector and keep track of the performed
permutation to be able to apply it to other vectors.

* `Span` A simple example descibing the `std::span` utility introduced in C++20 that provides a view over a contiguous sequence of elements and is a lightweight, non-owning reference to an array or a portion of an array.

* `Streams` A set of examples on streams, including binary i/o and string streams. In the example contained in `binary_io/` also show the use of an utility declared in `Utilities/string_utility.hpp`
that allows to read a whole file in a string.

* `Map` A simple program that shows the main functionalities of `std::map`

* `InitializerList` An simple example that explains how the use of `std::initializer_list<T>` can simplyfy construction of object that stores containers to be filled in the construction.



## What do I learn here? ##

- A few standard algorithms, of the many;
- The use of different comparison operators for ordered containers (an example of the Strategy design pattern).
- The `optional` class template, very useful to treat missing data without dangerous tricks;
- The new `filesystem` tools to transverse, exhamine and interact with the file system;
- Structured bindings and `std::tie` for tuples
