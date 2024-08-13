# One definition rule (ODR) #

This is an example of a very dangerous (and difficult to detect)
error.  It is related to the relaxed One Definition Rule (odr) when
dealing with template functions (or inline functions).

The one definition rule says that I can have in a program only one
definition of an object. For ordinary (not template, nor automatic
return, not inline, non constexpr) functions, it means that if during the linking
phase the compiler finds the definition of two identical functions it
gives an error. 
**This error may happen if you, by mistake, have defined the function in a header file, and not in the source file.**

However, the mechanism of instantiation of function templates (or inline functions) makes it
inevitable that instances of template functions with the same
arguments are compiled in all the translation units that instantiate/use
them, *(that's why function template and inline function definitions must go in the header file).*
Therefore, for those objects the ODR is relaxed.

Example:

file `myfun.hpp`:

```C++
template <class T>
double myfun(const T & x){...}

```


file `f1.cpp`:

```C++
#include "myfun.hpp"
...
double c=myfun(5.0) // will instantiate myfun(const double &)
                    //   the compiler will insert the corresponding machine code
                    //   in f1.o

```


file `f2.cpp`:

``` C++
#include "myfun.hpp"
...
double c=myfun(7.0) // will instantiate myfun(const double &)
                    // the compiler will insert the corresponding
                    // machine code in f2.o
```

Now we we link the object files (linking stage of the compilation process):

`g++ main.o f1.o f2.o -o main`-> the linker finds two definitions of `myfun(const double&)` in the object files.
 **It is not a problem, it just chooses the first one, a function template does not obey the ODR**.

The linker in this case will insert in the executable **only one copy of the compiled code for myfun(const double)**.
In normal cases, like the one just shown, this is perfectly fine since all instances are
identical. But unfortunately **the linker does not check if they are identical!**

In this example we show what can happen if one creates two function
templates with the same name and parameters but with a different
semantic (**something you should never, never, I said never, do!**).

In `three.hpp` I have the definition of a function template, called
`three`, that just multiplies the input by 3.0.  In `newthree.hpp` I have
the definition of a function template with the same (full qualified)
name, same parameters, but this time the function divides by three!

Now let's see how this can be VERY dangerous. In `fun.hpp` and `fun2.hpp`
I declare two functions, the first uses the first version of `three`,
the second is meant to use the second version. However, since the function
`three()` is not referred in the function declarations, I can avoid including
`three.hpp` and `newthree.hpp` in those files.  Indeed, they are included
only in the corresponding source files `fun.cpp` and `fun2.cpp`, where
function `three()` is actually used, and thus instantiated with the corresponding argument type (the same in both cases).

After compilation (which goes perfectly well) if you do
nm --demangle fun.o | grep three
you get

`00000000 W double three<double>(double const&)`

The W tells that that symbol is indeed resolved (defined), but it is
the instance of a function template: the function `three()` istantiated
with `T=double`. So `W` actually will tell the linker: ignore ODR here!

`nm --demangle fun2.o | grep three` will give the same result, since
also fun2 instantiates `three()` with `T=double`. Perfectly fine (or not? This function is different than the previous one!). 

If we have a main program that uses both fun and fun2 we get something really nasty.

The linker finds the function `three(double const &)` defined twice, but it is not an error
since we have two instances of a template function, *the linker does not know that the two instances are infact different*.

So, eventually in the main we have the code corresponding to only one
of the two versions of three(double const). Which one?  It depends on
the order the object files are given in the linking process!!

Run make, see what it does, and then run main and main2 and see what
happens!!

## HOW TO SOLVE THE PROBLEM ##

First of all let's say that this very nasty and difficult to detect error,
and in principle should not happen. The standard says that you can have
more than one identical instances of function templates, but they must
be identical. Unfortunately the linker has no way to check it.

Secondly, you should never write two function templates with the same
name and parameters. **The use of namespaces may also help to avoid name clashes.**

Thirdly, it is a good rule to include where you declare a function or a class that makes use of a template function the header file containing the definition of the
function template, even when it is not strictly necessary. Indeed, try to uncomment the `#include` statements in
`fun.hpp` and `fun2.hpp` and see what happens. Now, the main does not even
compile!  Why? Well the compiler finds two definition of the same function template, and this is an error!.
Relaxing the ODR rule applies to instances of function templates (i.e. template functions) not to the function template!

With this safety net, the compiler error tells you that you have a problem to sort out: you have to change the
name of one of the two functions (or use namespaces to separate them).

# What do I learn here? #
- Some technical details of the ODR rule;
- That you must be careful about name clashes, use long and significant names, and use namespaces;
- A little advice that may help avoiding this nasty situation (which anyway will not happen if you follow the previous advices).


