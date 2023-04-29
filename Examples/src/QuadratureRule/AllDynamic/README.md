# Dynamic loading of numerical rules and integrands #


This is a rather complex example, where we show the use of dynamic library leading to implement a plugin-type architecture and
dinamically load the integrand for our approximated integral. We also show how we can fit in the scheme the possibility of indicating the integral using `muParser` (for other examples and a descrition of `muParser` go in the `muParserInterface` folder).

Here we accomplish the following objectives:
- A code for univariate numerical integration (quadrature) capable of extending the applicable quadrature rule by loading "plugins" (shared libraries) dynamically. The user may specify which plugins are needed. To this purpose we use an object factory, specializing the factory in `GenericFactory`.
- We are able also to load the integrands from a library indicated by the user. Here we adopt the trick of C-linkage to be able to load the function with no need of an object factory (but, of course, this can be changed).
- We show how the proposed technique of dynamic loading of integrand function may be extended to function that are specified run time by reading the function expression from a file! To this purpose we use `muParser`. We recall that using a parser gives great flexibility but it is also computationally much less efficient than a compiled function.
- We use `GetPot` to read all information about the plugins, the library of user defined functions and to specify all the parameters
needed for the numerical integration.

To make it work:

* go in `Extra/muParser` and follow the instructions in `README_PACS.md` to install it.

* go in `../baseversion`, follow the instructions in the `README` file, and finally do

    make alllibs (maybe with DEBUG=no)
    make install

To compile the libraries:

    make alllib [DEBUG=no]

to compile the executable

    make exec [DEBUG=no]

We create several dynamic libraries

- `libRulesFactory.so` contains the factory of quadrature rules as a namespace variable. It must be linked to **all** code that uses the factory!
- `libudf.so` Contains the user defined functions, and also the wrapper for muParser. The latter makess use of the utility contained in `muParserFunction.hpp`.
- `libAdamsRules.so`, `libGaussRules.so` and `libMontecarloRules.so` are the plugin dynamic libraries that define a different set of quadrature rules.


## Things to note ##
- The use of two alternatives (one is commented) to register the rules into the factory as soon as the corresponding library is loaded. One uses an attribute of the compiler, the second, some helper object (you find the code in `GenericFactory`) whose creation registers the rule;
- The factory implements the Singleton design pattern, useful to avoid dangerous copies, and is implemented as a global (namespace) variable, so that we ensure that all code sees the same factory. Beware: the object code that provides the factory MUST be contained in a shared library! Otherwise you have problems due to the way the linker (at least the gnu linker) works (I avoid going into details here).
- The function `parsedFunction` reads the function expression from the file `parsedFunction.txt` (you cannot change the name, sorry). The muParser code is enucleated in a special class, called `muParserFunction`. To be able to use the parser properly, I need to create a static object inside the function. When the function `parsedFunction()` is called the first time, a `muParserFunction` object is created, which reads the expression from the file. The parser is then kept for the successive calls of the function. I had to do this way since `parsedFunction` must be a simple function with C-linkage. If you decide to use full fledged callable objects as integrands, which may include functors, you have more flexibility. But, then you need to have also a factory for the integrands.

# What do I learn here? #
- A rather complete code that makes use of dynamic loading of shared libraries to implement a plugin-type architecture.
- An example of use of singleton objects
- An example of interfacing with muParser
- Another example of GetPot usage.




 
- 




