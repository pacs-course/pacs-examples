# An example of reference bindigs and also of move semantic. #

The first part deals with binding of lvalue and rvalue
references. Note that if there exists a version of the function with a
rvalue reference as parameter, this version is preferred when the
argument is an rvalue. 

Indeed in C++ type categories are rather more complex than just
lvalues and rvalues. Let's however make the simplifying assumption
that the category rvalue indicates values that can be safely moved and
thus preferably bind to rvalue references (irrespectively of the fact
that they are eventually moved or just copied).

The second part concerns move semantic and it contains **two
versions** of a function taking a ``std::vector`` as parameter.

In the first version we have an overloading: two functions of name
``gvect`` are defined, one takes a ``const Vector &`` (Vector is an
alias to std::vector<double>) and the other a ``Vector&&``. The latter
applies to rvalues, and since ``std::vector`` implements move semantic
the argument will indeed be devoided of its content, which is moved to
the variable v in the function.

The second version uses the std::forward<T> magic to perform the same
semantic of the two previous functions with just one piece of code!.

The code is self-explanatory.

#What do I learn with this example?#

- Some basic concepts of move semantic. A way to optimize memory usage. Remember that the language provides move constructor and move assignment operator by default, so in most cases you have to do nothing and your class already implements move semantic. But there are rules  see 
[here](https://stackoverflow.com/questions/24342941/what-are-the-rules-for-automatic-generation-of-move-operations) and [here](https://images.app.goo.gl/hWVF7KGw46KnDTaq6).
- A reminder of reference bindings rule.
