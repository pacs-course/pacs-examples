# Two examples of Object Factory and Plugins#

An object factory is ... a factory of object, more precisely a factory
of polymorphic objects. Here an use of a object factory to implement a
plugin architecture. The two examples `FacoryPlugin` and `FactoryPluginII` in this subdirectory are
practically identical, they change only in the way the "plugin" fills
the object factory.

The idea of the pluging architecture is that you can extend the
functionalities of a code by dynamically loading shared libraries.

The basic incredients of a plugin architecture are

- One (or more) abstract classes that represent the common
  interface(s) of the various objects you may want to load dynamically
  in your code;
- One (or more) object factory(ies), which, as the name says, have the
  role of building the concrete objects that are provided by the
  plugins (dynamic libraries) once they are loaded;
- A way to load the builders of the concrete objects in the factory at
  the moment you load the plugin, so to have the possibility of
  creating the object(s) provided by the plugin.

In this example you the base class is the Polygon class which provides
the interface of a hyerarchy of polygon. I will create two dynamic
libraries which provide different polygons and load in a factory their
"builder", i.e. an object able to construct the polygons. The main,
when loading the dynamic library has at disposal, through the factory,
the possibility of using the polygons made available by the library.

The main files:

**Identical in the two subfolders**

* `loadPolyFactory.hpp` Declares a class whose role is to load, open,
  and hold dynamic libraries by reading the name form a file. It
  handles the resources (i.e. the libraries): the destructor closes
  the libraries. In other words, this class has the responsibility of
  handling the shared libraries dynamically linked by the code.
* `FactoryTraits.hpp|cpp` The header file declares the main types and,
more importantly, the variable `polyFactory` that will actually store
the factory thoughout the progam. Here for simplicity I have not used
a singleton pattern for the factory, as in the `GenericFactory`
example, (it is not strictly necessary). Instead I have given
`polyFactory` external linkage using the `extern` keyword: it is a
**global variable** (in a namespace though). The source file defines
the variable.  **Note**It is essential for the correct working of the
system that the object file `FactoryTraits.o` be stored in a shared
library that will be linked to the executable. The reasons are rather
technical, but this is the rule you have to follow.
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
  factory automatically when the library is loaded dynamically in the
  code.
  - in `FactoryPlugin` I have created a function with the attribute
  constructor in an anonymous namespace that loads the builder of each
  concrete object (which is here just a callable object that returns a
    unique pointer to the created polygon). Attribute constructore
  means: run this function when you load the library. This way you
  have automatic loading of factory when you load the
  library. Functions in anonympus namespaces are local to the
  translation units that define them (like static functions). This
  way, all plugins libraries will have *their own copy* of the
  function, and they can use the same name.
  - in `FactoryPluginII` I use instead a local variable whose creation
  loads the builders in the factory. When the library is dynamically
  loaded the code has to create the variable (even if it is not used
  anywhere else). Constructing it loads the builders in the factory. This way I can avoid the use of `__attribute__((constructor))`, which is not portable to all compilers (it works with `g++` and `clang++`).
* `Quadrilateral.hpp|cpp` Declares/defines another derived class. The
  corresponding object file will be stored in a dynamic library. All
  previous considerations apply also here.

The files holding the libraries (plugins) to be loaded is
`plugins.txt`. You may add other "polygon classes" to your code by
creating and loading other dynamic libraries, you do not have to
recompile the main program (nor the other libraries)!

# What do I learn from this example?#

The basic concepts of a plugin architecture. Plugins are commonly
used in big codes to enable adding functionalities without the need to
recompile the main program. And also to reduce the executable size: you load only what you need.
