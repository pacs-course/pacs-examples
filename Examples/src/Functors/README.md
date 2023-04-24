# Examples on function objects and lambdas #

An object function, or *functor*, is a callable object implemented as a
class or struct. A *callable object* is any object that can be called
using the `()` call operator: functions or a lambda expressions are other
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

# What do I learn here? #
- The concept of functors, which is very used by many standard algorithms.  The standard library also provides son base classes from which you can publicly derive your functor if it is a 
*unary function object*, or *binary function object* (i.e. a functur taking one or two arguments, respectively). 
They are called `std::unary_function` and `std::binary_function`, and are tempalte class templatized with the argument and return type of your functor. Inheriting from those classes is not necessary, but it can be of help if you use the functor inside standard algorithms.



