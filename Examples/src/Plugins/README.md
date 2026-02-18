# Two examples of an Object Factory and Plugin architecture   #

An object factory is ... a factory of object, more precisely a factory
of polymorphic objects. Here an use of a object factory to implement a
plugin architecture. The two examples `FacoryPlugin` and `FactoryPluginII` in this subdirectory are
practically identical, they change only in the way the "plugin" fills
the object factory.

The idea of the pluging architecture is that you can extend the
functionalities of a code by dynamically loading shared libraries.

The basic ingredients of a plugin architecture are

- One (or more) abstract classes that represent the common
  interface(s) of the various objects you may want to load dynamically
  in your code;
- One (or more) object factory(ies), which, as the name says, have the
  role of building the concrete objects that are provided by the
  plugins (dynamic libraries) once they are loaded;
- A way to load the builders of the concrete objects in the factory at
  the moment you load the plugin, so to have the possibility of
  creating the object(s) provided by the plugin.

In this example the base class is the `Polygon` class, which provides
the common interface of a hierarchy of polygons. I will create two dynamic
libraries providing different polygons and load in a factory their
"builder", i.e. an object able to construct the concrete polygons. 
The libraries are loaded dynamically in the main program using the facility `LoadLibraries` provided 
in folder `LoadLibraries`. 

** You have first to go in the folder `LoadLibraries` and do `make alllibs; make install` to install the library libloadlibs.so in the Examples/lib directory.**


The main program will then be able to upload the plugins (dynamic libraries) indicated in the file
`plugins.txt` and have at disposal, through the factory,
the possibility of using the polygons made available by the loaded plugins.

The main files:

**Identical in the two subfolders**

* `loadPolyFactory.hpp` Its just rename the utility in defined `LoadLibraries.hpp` for backward compatibility.
* `FactoryTraits.hpp|cpp` The header file declares the main types and,
more importantly, the variable `polyFactory` that will actually store
the factory throughout the program. Here for simplicity I have not used
a singleton pattern for the factory, as in the `GenericFactory`
example, (it is not strictly necessary). Instead I have given
`polyFactory` external linkage using the `extern` keyword: it is a
**global variable** (in a namespace though, so technically is a namespace variable). 
* `Polygon.hpp|cpp` Declares/defines the base class. Note that the
  corresponding object all "plugin" dynamic libraries must be linked
  against the object file. Otherwise the derived classes will not be
  properly formed and you get a nasty runtime error. Look at the
  Makefile, the lines where I define the rule to create the dynamic
  libraries.

**Different in the two subfolders**

* `TriangleAndSquare.hpp|cpp` Declares/defines 2 concrete classes
  derived from `Polygon`.  The corresponding object file will be
  stored in a dynamic library that will be used as plugin,
  i.e. dynamically loaded.  The two versions differ only in the source
  file where I choose two different ways to load the objects in the
  factory automatically when the library is loaded dynamically in the code.
* `Quadrilateral.hpp|cpp` Declares/defines another derived class. The
  corresponding object file will be stored in a dynamic library. All
  previous considerations apply also here.
+ in `FactoryPlugin` I have created in an anonymous namespace a function with the *constructor attribute* that loads the builder of each
  concrete object (which is here just a callable object returning a unique pointer to the created polygon). Attribute constructor means: run this function when you load the library. This way you
  have automatic registration of the object in the factory when you load the
  library. Functions in anonymous namespaces are local to the
  translation units that define them (like static functions). This
  way, all plugins libraries will have *their own copy* of the
  function, and can use the same name with no problem.
- in `FactoryPluginII` I use instead a local variable whose creation
  loads the builders in the factory. When the library is dynamically
  loaded the code has to create the variable (even if it is not used
  anywhere else). Constructing it loads the builders in the factory. 
  This way I avoid the use of `__attribute__((constructor))`, 
  which is not portable to all compilers (it works, however, with `g++` and `clang++`).

The file holding the libraries (plugins) to be loaded is
`plugins.txt`. You may add other "polygon classes" to your code by
creating and loading other dynamic libraries, *you do not have to
recompile the main program (nor the other libraries)!*, this is the beauty of plugins.

## Note ##
It is essential for the correct working of the
system that the object file `FactoryTraits.o` be stored in a shared
library that will be linked to the executable. The reasons are rather
technical, but this is the rule you have to follow to make things simple.

# What do I learn from this example? #

- The basic concepts of a plugin architecture. Plugins are commonly used in large codes to enable adding functionalities without the need to recompile the main program. And also to reduce the executable size: you load only what you need.
- How to use the `LoadLibraries` utility to load shared libraries.
- Two different ways of registering objects in an object factory automatically.