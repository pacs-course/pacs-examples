#The new list-initialization and aggregate initialization goodies#

Since C++11 we can initialize class objects, using the {} syntax.  It
it a replacement of the `()` syntax that has been introduced to have a
uniform syntax for initialization. Indeed the `{}` syntax existed for
aggregates (see *note*) already in C language. The main difference is that with the
`{}` syntax implicit conversion is suppressed if is narrowing (loss of precision).

Furtermore, for almost all containers, you can also use the syntax
```
vector<double> a={1.2,3.4,7.0};
```
The construct at the right is called **initializer list**.
And that can be done also for aggregates.

Here we show a few examples.

**NOTE 1** Beware that to maintain compatibility with older versions of the language
```
std::vector<int> a(10,9);
```
defines a vector of 10 elements all initialized with 9. While,
```
std::vector<int> a{10,9};
```
defines a vector of 2 element: 10 and 9.  With containers prefer the syntax
```
std::vector<int> a={10,9};
```
so you avoid ambiguity.

**NOTE** Since C++17 aggregates can have base classes, provide
inheritance is public and base classes are themselves aggregates. You
see an example also of this new feature.
