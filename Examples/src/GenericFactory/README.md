# A generic object factory #

Based on a simplified version of the generic object factory of the book
of Andrei Alexandrescu *Modern C++ Design: Generic Programming and Design Patterns, Addison-Wesley, 2001*.

We also give an example of a generic proxy class that may be used to
register objects in the Factory automatically using the technique of local variables (seen in `FactoryPlugin`)


Do ``make exec`` to produce the executable that tests the Factory. The `Utility` folder should contain a link to `Factory.hpp` and `Proxy.hpp` so you
do not need to run `make install` to have those facilities available to other examples (you have to install the `Utilities` instead).

# What do you lean with this example #
- An example of a generic implementation of the *object factory* design pattern. The key for a plugin-type architecture and, in general, useful when you have to select an element of a hierarchy of polymorphic object run time;
- How the same class template can be used to create a "factory of functions";
- The use of generic programming to build a model for various implementations;
- The use of *variadic templates* to pass a variable number of arguments to a function;
- The creation of *exceptions* using the tools provided by the standard library;
- The *Singleton* design pattern. Object factories are indeed often (but not necessarily) implemented as Singletons since you want to be sure that the same factory provided objects in all your code. 
- The use of *static variables* to register objects in the factory. This is a very useful technique that is used in many libraries. It has been made thread safe since C++11.

# How to use this example #
- The `Factory.hpp` file contains the implementation of the Factory. The `Proxy.hpp` file contains the implementation of the Proxy class. The file `main_testAbstractFactory.cpp` contains an example of the use.

# A Note #
Remember that just making a class a Singleton does not guarantee having only one instance of the object in your code. But it provides an additional safeguard. The safest way to be sure to have a single factory object in your code is to make you factory a Singleton *and* a global(namespace) variable, as shown in the example `NumericalQuadrature`.