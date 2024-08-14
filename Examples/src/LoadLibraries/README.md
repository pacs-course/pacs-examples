# An utility to load and store libraries dynamically #

It is an utility, in the form of a library that enables you to
- To load dynamically a set of shared library and store the corresponding handlers in a common repository
- Close all libraries when it exits out of scope: it implements the RAII principle.
- You may add a library one at a time or read them from a file
- The dynamic library loads -ldl automatically, so you do not have to specify it when creating your executable.


*Note* When going out of scope the code closes the libraries. So the lifetime of a `LoadLibraries` object should exceed  that of the code that uses the libraries! Therefore, it is normally better to use this tool in the main program, or store the object in a static variable.
 
 
`make alllibs` creates both static and dynamic libraries
`make install` to install the libraries in the 
`make exec` produces a test

** The test is also an example of factories! Indeed it uses a generic factory to store functions. Have a look! **


# What do I learn here? #
- The use of a common repository can help to handle dynamic libraries loaded in a plugin architecture.
