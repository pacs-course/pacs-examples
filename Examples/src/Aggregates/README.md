# The uniform (brace) initialization #

Since C++11 we can initialize class objects, using the `{}` syntax (sometimes called *brace initialization*).  It
it a replacement of the `()` syntax introduced to have a uniform way of initialize objects. Indeed the `{}` syntax existed for
aggregates (see *note*) already in C language. 


A main difference with using '()' is that with the
`{}` syntax implicit conversion is suppressed if it is narrowing (i.e. if it induces loss of precision).

Furtermore, for almost all containers, you can also use the syntax
```
std::vector<double> a={1.2,3.4,7.0};
```
which maps to the constructor of `std::vector` that takes an `std::initializer_list<double>`.
The construct at the right is called **initializer list** and indeed it is an `std::initializer_list<T>` (with `T=double` in this case).

And, since C++17, you can also use the syntax

```c++
std::vector a{1.2,3.4,7.0};
```
Brace initialization gives the possibility of initializing object with a common syntax. This has been achieved in practice in different ways.

- For aggregates  we had already aggregate initialization, which allows to use braces to initialize an aggregate object. In C++ (in fact since C++17) the concept of aggregate has been extended to account also inheritance (see `NoteOnAggregate.md`);
- For plain old data the language now supports, for intance, `double c{10.0}`, to initialize `c` to `10`, yet (for backward compatibility) `double c(10.0)` is still valid;
- For more general classes, if you have a constructor that takes, for instance, a `int` and a `double`, you can now initialize an object of the class with `Myclass m{10,-9.9}` or even (if the constructor is not explicit)
`MyClass m={10,-0.9}`. Clearly, the old-style `MyClass m(10,-9.9)` is still valid. A main difference is that now `MyClass m{}` calls the default constructor, while `MyClass m()` is an error (and this is definitely  useful in generic programming!);
- For containers of the standard library, brace initialization has been introduced by creating constructors (and assignement operator) that accept a `std::initializer_list<T>`. You can use the same trick if you are creating your own container class (see the example).

Anyway, irrespectively on how it has been obtained, we may now use braces to initialize objects, if we want.

** If you are programming in modern C++ you should use the brace initialization syntax. ** I have repported the old style syntax only for completeness and to avoid confusion. 

**NOTE** Beware that to maintain compatibility with older versions of the language
```
std::vector<int> a(10,9);
```
defines a vector of 10 elements all initialized with 9. While,
```
std::vector<int> a{10,9};
```
defines a vector of 2 elements: 10 and 9.  With containers prefer the syntax
```
std::vector<int> a={10,9};
```
so you avoid ambiguity.


# What do I learn here? #
- Uniform initialization;
- Some features of aggregates (see `NoteOnAggregate.md` for details).
- How to initialize containers with a list of elements.
