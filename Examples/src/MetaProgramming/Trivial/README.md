#An example to show trivial trivially copyable and standard layout objects#

*Trivially copyable* and *standard layout* are important type category (also if the former id not an official one), see [type categories](https://en.cppreference.com/w/cpp/named_req) if you are interested in the different named requirements of the C++ language.

 An object trivially copiable type can be copied by `std::memcpy` or `std::memmov` and trivially serialised, so you can put the content into a buffer and transfer it with 
 `MPI_SEND` directly.
 
 A standard layout object can be interfaced with other languages (like C) in a simple way.
 
 The rules that specify those categories may be a little technical. But we have some type traits that allow us to interrogate if a type is trivually copiable or has standard layout. We show here some example of their use and also which operations can be safely made in the case an object of of a type that belong to one of those categories.
 
 I also use `std::is_aggregate` to test whether some types are aggregate. Aggregates are (almost) a subset of standard layout objects.
 
 #What do I learn here?#
 - The use of some type traits that aneble to interrogate some properties of types;
 - An example of some capabilities of standard layout and trivially copyable objects.
 
  