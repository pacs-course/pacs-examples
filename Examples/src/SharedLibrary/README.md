#A very simple example of the use of shared libraries#


The command `exec.sh` compiles and runs the example

This is just an example that creates two "relases" of a shared library an and sets up the technique used for versioning in Linux. 
By changing some links you may use the old or new release of the library, without recompiling the main program. That's what happens when you install a new release of a library in your PC.

The complexity of the handling of versions and releases is of course necessary in production code. If the library is used only locally you can avoid it.

In `DynamicLoading` we show another characteristics of shared libraries: the fact that you can dynamically load them, and extract objects contained in the library run time.

#What do I learn here?#
- Some basic concepts of shared libraries. 