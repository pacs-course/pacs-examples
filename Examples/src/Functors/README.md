#Examples on function objects and lambdas#

An object function or *functor* is a callable object implemented as a
class or struct. A *callable object* is any object that can be called
using the `()` call operator: a function or a lambda are other
examples of callable objects.

Functors are useful particularly when their behaviour depends on the
state of the object, which you may change depending on the needs.

Here we show some simple examples, to explain the technique, which is indeed rather simple: a functor is a class that implements the call operator:

``` C++
ReturnType operator()(Args...)
```

Their name is related to the fact that they may behave like functions

``` C++
AFunctor f;
// maybe I change the state of f
auto x = f(x); // calls the call operator
```
