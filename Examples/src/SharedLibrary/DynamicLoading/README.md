#Dynamic loading of shared libraries and functions#

We will see more complex use of dynamic loading of libraries (the basis of a plugin architecture) later on. Here I show how it is rather easy to retrieve objects dynamically from a synamically loaded shared library in the case of functions.

The trick is to avoid name mangling by using `extern C`.

#What do I get from this example?#
- How to extract a function from a shared library loaded dynamically.