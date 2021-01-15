# A generic object factory #

Based on a simplified version of that of the book
of Andrei Alexandrescu *Modern C++ Design: Generic Programming and Design Patterns, Addison-Wesley, 2001*.

We also give an example of a generic proxy class that may be used to
register objects in the Factory automatically using the technique of local variable (seen in `FactoryPlugin`)


Do ``make exec`` to produce the executable that tests the Factory. The `Utility` folder should contain a link to `Factory.hpp` and `Proxy.hpp` so you
do not need to run `make install` to have those facilities availabel to the other examples. If it is not so, run `make install`.

#What do you lean with this example#
- An example of a generic implemetation of the *object factory* design pattern. The key for a plugin-type architecture and, in general, useful when you have to select an element of a hierarchy of polymorphic object run time;
- The use of generic programming to build a model for various implementations;
- The use of *variadic templates* to pass to a function a variable number of arguments;
- The creation of *exceptions* using the tools provided by the standard library;
- The *Singleton* design pattern.
