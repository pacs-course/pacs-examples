# Decltyle and declval #

`decltype()` returns the type of an expression. But what if I want to test the type of the result of the call to a non-static method of a class?
I need to have an object of the class to call the method. But it does not make sense to create an object if you just want to know the return type of a member function!

C++ allows to overcome this difficulty calling the method in a "unevaluated context".

A little explanation first of what this unevaluated context means. Well, when computing an expression you get a value. For instance,

```c++
double x = a +3.0;
```
is clearly a statement in an evaluated context: the *value* of `a` is summed to the *value* of the literal `3.0` to obtain the *value* used to initialise `x`. In c++ a value is always associated with a type (in this case we do not know the type of `a`, but it must be a type convertible to a double, otherwise we get a compilation error).

Ok, but look now at this statement, where I want to obtain the size of a double (in bytes)

```c++
int i = sizeof(double);
```

The `double` argument of `sizeof` is obvously not a value! It means that the argument of `size_of` is in an `unevaluated context`, the value is irrelevant and not used, that's why we can just write `double` and we do not need to provide a value. Note that this is completely different than a ordinary function

```c++
double f (double x){ ... do something '}
.
.
f(double); // Error: it does not make sense!!
```
Function `f` wants an argument in evaluated context: it must be something from which we may get a value!. 
Even if I do `sizeof(3.0)`, the *value* '3.0' is not used, the compiler needs t know that it is a double. Again, I can also do `sizeof(double())`, by calling the constructor of a double explicitly: no double is constructed, since I am in an *unevaluated context*.


Also `decltype`, as `sizeof` operates in an unevaluated context.
This useless statement

```c++
decltype(double) a;
```
is valid, though uselesse since `a` is now a double and I could have simply written `double a`.

Ok, why all this discussion? Well let's look at the following situation

```c++
struct A
{
  double f(double x)
}
```

I want to automatically deduce the type returned by `A::f(double)` using `decltype` (extracting the type returnred by a member function may be useful in generic programming)
Of course I can simply do 

```c++
decltype(A().f(double())) b;

```

Since the expression inside `decltype` is in unevaluated context no object of type `A` is created, the member function `f` is not called, and no double is created. The compiler extracts the type of the value the the extression would create is an object of type `A` where created (with the default constructor) and the method `f(double)` would have been called. The variable `b` will be here of type `double`, since the member function returna a `double`.

Fine, but what the role of `declval`? Well, the probelm is that in a generic setting the previous statement has a problem. The class `A` (which is a generic program may depend on a template parameter) may not have a default constructor! In this case the statement fails, because even if `A()` is not evaluated, it must be well formed.

We can get around this problem by using the `declval<>` magic. How it works is very tecnical, but the use is relatively simple. `declval<A>()` is an expression
that can be used **only in an unevaluated context** and returns "something" (in fact a revalue reference) that allows to do the trick:

```c++
decltype(std::declval<A>().f(double()) b;
```

**it works even if A is not default constructible**.


These example show the use of `decltype` and `declval` experimenting them with two classes, one default constructible and the other not default constructible. The default constuctor of the first writes something to the screen just to show that indeed in an unevaluated context the object is not constructed (indeed the message doeas not appear on the screen!)

# What do I learn from this example #

- The use of a very technical feature, useful only in the case of heavy generic programming, not very relevant in more standard programming;
- Some other type traits of the standard library;
- The `typeinfo` tools to interrogate run-time the type of a variable.




