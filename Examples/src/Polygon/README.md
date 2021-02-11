#An example of polymorphism.#

A set of classes that derive from an abstract `Polygon` class. The method `showme` is virtual and allows to print  at run time which Polygon we are using.

#What do I learn here#
- The use of public inheritance and polymoprhism to create a hyerarchy of objects that share the fact that "they are a polygon".
- A simple example of object factory with the use of variadic templates;
- The classes have been made clonable to allow them to be composed into a container.