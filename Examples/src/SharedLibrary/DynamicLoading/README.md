# Dynamic loading of shared libraries and functions #

You have more complex use of dynamic loading of libraries (the basis of a plugin architecture) on other examples. Here I show how it is rather easy to retrieve simple function dynamically from a dynamically loaded shared library. The trick is to avoid name mangling by using `extern C`. 


In examples contained in other folders we show a more elegant way of handling function dynamically loaded from shared libraries using a "function factory". 

# What do I get from this example?  #
- How to extract a function from a shared library loaded dynamically in a simple way
- How to build a simple factory for functions
- The flexibility of std::function
- The use of a map to maps integers to strings