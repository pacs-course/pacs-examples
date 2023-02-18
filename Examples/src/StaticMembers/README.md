# Example of use of static members #

A class that stores a double, counts how many objects of that class
   have been created and is able to access the average of the value of
   all elements of that class in the program. It is an example of use
   of non constant static members to monitor something related to all
   objects of the class.
   
Often, static member of a class are const variable (or even constexpr,
constexpr implies static) that represent quantities common to all
members of the class.  For instance, a class of a `Polygon` family may
have as static member variable the number of vertices of specific
Polygons: for `Triangle` is 3, for a `Square` is 4 ...

In modern C++ you can initialize in-class 
static variable members (before c++11 ou could do it only with ints),
but **you have to declare them inline**.

Only to recall the issue:

If you want to initialise a static member variable

- Before c++11 (valid also now)

**in the header file Foo.h**

```c++
class Foo
{
...
static double x; // Declaration only
};
```
**In the source file Foo.cpp**

```
double Foo::x = 6.0; Definition and initialization
```

- Since c++11

```
class Foo
{
...
inline static double x=6.0; // Definition and initialization
};
```

If the static variable is `constexpr` you can avoid inline 
since `constexpr` implies `inline` (since C++14).


# What do I get from this example? #
- A possible use of static variables to track statistics of the class objects;
- The issue of initialization of static member variables.


