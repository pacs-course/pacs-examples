#An utility to load and store dynamically libraries#

It is an utility, in the form of a library that enables you to
- To load dynamically a set of shared library and store the handler in a common repository
- Close all libraries when it exits out of scope
- You may add a library one at a time or read them from a file

*Note* When going out of scope the code closes the libraries. So the lifetime of a `LoadLibraries` object should exceed 
that of the prt of the codes tha tuses the libraries! Therefore, it is normally better to use this tool in the main program.
 
 
`make alllibs` creates both static and dynamic libraries
`make install` to install the libraries in the 

The dynamic library loads -ldl automatically, so you do not have to specify it when creating your executable.

#What do I learn here?#
- The use of a common repository can help to handle dynamic libraries loaded in a plugin architecture.
