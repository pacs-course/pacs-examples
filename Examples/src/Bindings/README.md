# An example of reference bindigs and also of move semantic. #

The first part deals with binding of lvalue and rvalue
references. Note that if there exists a version of the function with a
rvalue reference as parameter, this version is preferred when the
argument is an rvalue. 

Indeed in C++ type categories are rather more complex than just
lvalues and rvalues. Let's however make the simplifying assumption
that the category rvalue indicates values that can be safely moved and
thus preferably bind to rvalue references (irrespectively of the fact
that they are eventually moved or not).

The second part concerns move semantic and it contains **two
versions** of a function taking a ``std::vector`` as parameter.

In the first version we have an overloading: two functions of name
``gvect`` are defined, one takes a ``const Vector &`` (Vector is an
alias to std::vector<double>) and the other a ``Vector&&``. The latter
applies to rvalues, and since ``std::vector`` implements move semantic
the argument is indeed devoid of its content, which is indeed moved to
the variable v in the function.

The second version uses the std::forward<T> magic to perform the same
semantic of the two previous function with just one piece of code!.

The code is self-explanatory.
