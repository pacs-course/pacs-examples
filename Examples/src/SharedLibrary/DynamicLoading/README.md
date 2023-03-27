#Dynamic loading of shared libraries and functions#

You have more complex use of dynamic loading of libraries (the basis of a plugin architecture) on other examples. Here I show how it is rather easy to retrieve simple function dynamically from a dynamically loaded shared library. The trick is to avoid name mangling by using `extern C`.

In the example LoadLibraries. you find how to use a Function Factory for this purpose. A function factory is just an object factory for functions. With a factory you have extra flexibility (possibility to load objects from more than one library, and you do not need  `c` linkage).

#What do I get from this example?#
- How to extract a function from a shared library loaded dynamically in a simple way
- How to build a simple factory for functions
- The flexibility of std::function
- The use of a map to maps integers to strings