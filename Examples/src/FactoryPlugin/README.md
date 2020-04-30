# Object Factory and Plugins#

An object factory is ... a factory of object, more precisely a factory
of polymorphic objects. Here an use of a object factory to implement a plugin architecture.

Things to note

* `loadPolyFactory.hpp` Declares a class whose role is to load, open, and hold dynamic libraries by reading the name form a file. It handles the resources (i.e. the libraries): the destructor closes the libraries.
* `FactoryTraits` Defines the main types and, more importantly, the variable `polyFactory` that will actually store the factory thoughout the progam. Here I have not used a simngleton for the factory, for simplicity. I rely on the fact that this file is included in the main file, so the variable will be shared by all utilities using it. Note that I have used the c++17 syntax for namespace and global variables by declaring `polyFactory` inline.
* `Polygon.hpp` Declares the base class. Note that the corresponding object file it must be linked agains all plugin dynamic libraries. Otherwise the derived classes will not be properly formed and you get a nasty runtime error. Look at the Makefile, the lines where I define the rule to create the dynamic libraries.
* `TriangleAndSquare.hpp` Declares 2 derived classes. The corresponding object file will be stored in a dynamic library. Note the way I load the objects in the factory: I have created a static function with attribute constructor. This attribute means: run this function when you load the library. This way you have automatic loading of factory when you load the library. Static functions are deprecated in C++. You may want to be more c++ style by creating an object, as done in other examples. Yet, I think that the solution with a static function is simpler. The function must be declared static since it must have local linkage (all plugins libraries will have *their own copy* of the function, with the same name!).
* `Quadrilateral.hpp` Declares another derived class. The corresponding object file will be stored in a dynamic library.
* `Makefile` Note the use of the option `-export-dynamic`. This option is needed when linking the main program. It is *essential*, since code in the dynamic libraries
refers to objects (actually only one:`polyFactory`) not provided by the library but by the main program. If you omit it you get a nasty segmentation fault.

The files holding the lbraries (plugins) to be loaded is
`plugins.txt`. You may add other "polygon classes" to your code by
creating and loading other dynamic libraries, you do not have to
recompile the main program (nor the other libraries).

