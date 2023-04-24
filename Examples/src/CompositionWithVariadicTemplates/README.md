# Variadic templates for composition via inheritance #

This is an interesting example on using variatic template to construct complex objects from simple components via inheritance. 
It can be seen a template version of a Builder design pattern.

By adding template arguments from which a class inherits you can expand the funcionality of a class. With variadic templates you may add
as many components as you wish! Indeed, this technique implements a compositional design pattern that uses public
inheritance from an arbitrary number of template arguments.

Some care should be taken, the basic components should be as simple as possible, and default constructible.

# What do I learn here? #

- A use of variadic template, composition by public inheritance: a peculiar implementation of the Builder design pattern;
- How to use concepts with variadic templates;
- How to use concepts to check that the template arguments are all default constructible.
