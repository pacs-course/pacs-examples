# A generic object factory #

Based on a simplified version of the generic object factory of the book
of Andrei Alexandrescu *Modern C++ Design: Generic Programming and Design Patterns, Addison-Wesley, 2001*.

We also give an example of a generic proxy class that may be used to
register objects in the Factory automatically using the technique of local variables (seen in `FactoryPlugin`)

Do ``make exec`` to produce the executable that tests the Factory. The `Utility` folder should contain a link to `Factory.hpp` and `Proxy.hpp` so you
do not need to run `make install` to have those facilities available to other examples (you have to install the `Utilities` instead).

## The Factory class ##

```c++
template <typename AbstractProduct, typename Identifier,
          typename Builder = std::function<std::unique_ptr<AbstractProduct>()> >
class Factory
```

`AbstractProduct` is the type of the base class of all concrete objects the factory is meant to produce. `Identifier` is the type used to identify the objects registered in the factory. `Builder` is the type of the tool used to build a concrete object. It is defaulted to a function wrapper returning an unique pointer to `AbstractProduct`.

The main methods are
- `void add(Identifier const &, Builder_type const &)` that register an object to the factory by passing its identifier and builder.
- 
```c++
  template <typename... Args>
std::unique_ptr<AbstractProduct> create(Identifier const &name,
                                          Args &&...args) const;
``` 
It return` a unique pointer to the object indicated by `name` and passing
possible arguments to the builder (clearly the builder must be capable to accept the arguments).

The same class may also be used as a **function factory**, i.e. a factory of callable objects with a given signature.
To this aim, the `Builder` should be a function wrapper of the correct type, and you should use the method

```c++
  Builder get(Identifier const &name) const;
``` 
to extract the desired function, given a choese identifier.

The factory is implemented as a **Singleton**. Therefore, to use it in your code you have to use the static method

```c++
  static Factory &instance();
```
to get the instance of the factory. For example

```c++
  auto&  myFactory = Factory<A,I,B>::Instance();
```

In the file `main_testAbstractFactory.cpp` you find an example of usage.



## The GenericFactory:: Proxy class ##
The class
```c++
template <typename Factory, typename ConcreteProduct> class Proxy
```
is a helper class whose constructor registers a `ConcreteProduct` in the 
factory `Factory`, instance of the `Factory` class template shown above.
It may be used, as done in the `NumericalQuadrature` examples, to automatically register objects in the object factory when loading a dynamic library. To that
purpose, the source file with the definitions of the concrete objects `Concrete1`, `Concrete2`...
should contain, assuming the identifier is a string, something like

```c++
namespace {
	GenericFactory::Proxy<Factory,Concrete1> c1{"c1"};
	GenericFactory::Proxy<Factory,Concrete2> c2{"c2"};
}
```

or
```c++
static 	GenericFactory::Proxy<Factory,Concrete1> c1{"c1"};
static 	GenericFactory::Proxy<Factory,Concrete2> c2{"c2"};
```

to oblige the compiler to construct `c1` and `c2` and thus register the objects with identifiers "c1" and "c2", respectively.

# What do you lean with this example #
- A possible generic implementation of the *object factory* design pattern. It is the key for a plugin-type architecture and, in general, useful when you have to select an element of a hierarchy of polymorphic object run time;
- How the same class template can be used to create a "factory of functions";
- The use of generic programming to build a model for various implementations;
- The use of *variadic templates* to pass a variable number of arguments to a function;
- The creation of *exceptions* using the tools provided by the standard library;
- The *Singleton* design pattern. Object factories are indeed often (but not necessarily) implemented as Singletons since you want to be sure that the same factory provided objects in all your code. 
- The use of *static variables* to register objects in the factory. This is a very useful technique that is used in many libraries. It has been made thread safe since C++11.

# How to use this example #
- The `Factory.hpp` file contains the implementation of the Factory. The `Proxy.hpp` file contains the implementation of the Proxy class. The file `main_testAbstractFactory.cpp` contains an example of the use.

# A Note #
Remember that just making a class a Singleton does not guarantee having only one instance of the object in your code. But it provides an additional safeguard. The safest way to be sure to have a single factory object in your code is to make you factory a Singleton *and* a global(namespace) variable, as shown in the `NumericalQuadrature` example.