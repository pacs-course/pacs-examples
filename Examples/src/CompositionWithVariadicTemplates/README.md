# Variadic templates for composition via inheritance #

This is an interesting example on how to use variatic template as a means
of constructing complex objects from simple components. It is a tempalte version of a Builder design pattern.

By adding template arguments from which a class inherits you can expand the funcionalyty of a class. With variadic templates you may add
as meny components as you whish! Indeed this technique implements a compositional design pattern that uses public
inheritance from an arbitrary number of template arguments.

#What do I learn here?#

The use of variadic template, composition by public inheritance, a peculiar implementation of the Builder design pattern
