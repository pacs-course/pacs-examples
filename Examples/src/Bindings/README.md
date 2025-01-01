# An example of reference bindigs and move semantic basics. #

The first part of the code in `main.cpp` deals with binding of lvalue and rvalue  references. 

The general rule is that if there exists an overload of a function with a
rvalue reference as parameter, this version is preferred when the
argument is a rvalue. 

C++ type categories are indeeed rather more complex than just
lvalues and rvalues. We make the simplifying (but basically correct) assumption
that the category (non const) rvalue indicates values that can be safely moved and
thus preferably binds to rvalue references (irrespective of the fact  that they are eventually *moved* or not!). We also recall that with *moving* we mean that the dynamic data contened in the object is moved to another location in memory, and the original object is left in a state of *empty* (i.e. it is not destroyed). For this to be possible, a special move constructor and move assignment are needed. If not present, the move becomes equivalent to a copy.

I also recall that move can be different from copy only for objects that, directly or indirectly, handle memory dynamically. For example, a `std::vector` is a good candidate for move semantic, while a `std::array` is not. If you try to move an `std::array` you will see that the move constructor is equivalent to the copy constructor.

In the example we present first the "pre c++11 " situation, with just lvalue references. In this case, const lvalue references can bind to any category,
while non-const lvalue references bind only and preferably to non const lvalues. Normally, we use non-const lvalue reference when
we want to possibly modify the argument and pass back the modified value to the caller.

In the second set of function I show the full overloads with all possible reference type!. Normally you don't 
need all of them, and you are indeed encouraged to comment out some and see what happens! Here, you may see that the overload with a r-value reference
bind only (and preferably) to rvalues.


The second part of the code concerns perfect forwarding, and it contains **two versions** 
of a function taking a `std::vector` as parameter (*one of the two should be commented out*).

In the first version we have an overloading similar to what seen in the first part: two functions of name
``gvect`` are defined, one takes a `const Vector &` (Vector is an
alias to `std::vector<double>`) and the other a `Vector&&`. The latter
applies to rvalues, and since `std::vector` implements move semantic
the argument is indeed moved to
the variable `v` in the function and returns void of content!.

The second version uses the `std::forward<T>` magic to perform the same
semantic of the two previous functions with just one piece of code!.

Clearly you cannot have both versions at the same time. So to test one or the other you have to comment/uncomment the one you want to test.
You find the instruction in the main.cpp file.

The rest of the code is self-explanatory.

## Eliminating compiler warning ##
In this example I define a few functions whose parameter is not used. Since I activate the compiler option `-Wall` I would get a hideous warning. So, I have taken the occasion to show 
you how to suppress compiler warnings with a pragma:

- `#pragma GCC diagnostic push` just stores the current warnings setting, so you can recover it afterwards;  
- `#pragma GCC diagnostic ignored "-Wunused-parameter"` is the pragma that disable the unwanted warning;
- `#pragma GCC diagnostic pop` resets warning setting to the previous situation.

**A note: a dirty trick to avoid the warning**. If you don't want to use the pragma, you can use the following trick: just add a line of code that uses the parameter with zero computational cost. It is sfficient to cast it to void. For example:

```C++
// a function that does not use the parameter
double fun(double x) {
  (void) x;// this line avoids the warning
  ...
}
```

# What do I learn with this example? #
- A reminder of reference bindings rule.
- Some basic concepts of move semantic, a way to optimize memory usage.
- How to suppress compiler warnings.
- How to use `std::forward<T>` to perform perfect forwarding.