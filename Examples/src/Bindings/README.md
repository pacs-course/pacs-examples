# An example of reference bindigs and also of move semantic. #

The first part deals with binding of lvalue and rvalue
references. Note that if there exists a version of the function with a
rvalue reference as parameter, this version is preferred when the
argument is an rvalue. 

C++ type categories are rather more complex than just
lvalues and rvalues. We make the simplifying (but basically correct) assumption
that the category rvalue indicates values that can be safely moved and
thus preferably bind to rvalue references (irrespectively of the fact
that they are eventually moved or not).

The second part cof the code oncerns move semantic and it contains **two versions** 
of a function taking a `std::vector` as parameter.

In the first version we have an overloading: two functions of name
``gvect`` are defined, one takes a `const Vector &` (Vector is an
alias to `std::vector<double>`) and the other a `Vector&&`. The latter
applies to rvalues, and since `std::vector` implements move semantic
the argument will indeed be devoided of its content, which is moved to
the variable `v` in the function.

The second version uses the `std::forward<T>` magic to perform the same
semantic of the two previous functions with just one piece of code!.

Clearly you cannot have both versions at the same time. So to test one or the other you have to comment/uncomment part of the cose.
You find the instruction in the main.cpp file.

The rest of the code is self-explanatory.

## Eliminating compiler warning ##
In this example I define a few functions whose parameter is not used. Since I activate the compiler option `-Wall` I would get a hideous warning. So, I have taken the occasion to show 
you how to suppress compiler warnings with a pragma:

- `#pragma GCC diagnostic push` just stores the current warnings setting, so you can recover it afterwards;  
- `#pragma GCC diagnostic ignored "-Wunused-parameter"` is the pragma that disable the unwanted warning;
- `#pragma GCC diagnostic pop` resets warning setting to the previous situation.



  

#What do I learn with this example?#

- A reminder of reference bindings rule.
- Some basic concepts of move semantic. A way to optimize memory usage. 