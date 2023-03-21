# An example about explicit instantiation of templates #

Explicit instantiation allows to precompile instances of class templates
for values of the template parameter that are considered most common. 

In general the object code containing the instances is then stored in a library.
here called `libmylib.so`.

If you use `nm --demangle` you may note that it stores the symbols for
all the classes/functions that have been explicitely
instantiated. Those symbols will be loaded into your code at linking
stage, avoiding useless recompilations of function/class templates that are
used very often.

# What do I learn here? #
- A technique that may reduce compilation time for widely used template instances.
