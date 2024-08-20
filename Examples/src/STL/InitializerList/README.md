# Using initializer_list to initialise a user defined class (and more) #


`initializer_list<T>` is a lightweight proxy object that provides access to an array of objects of type `const T`. All dynamic standard containers can by initialised/assigned by a initializer_list.
An inizialiser list can be constructed using *brace initialization*, and this  allows to implement the so called *uniform (brace) initialization* on standard containers, by which `vector<double> v{1.,2.}` is a vector of 2 doubles, `1.` and `2.`. You can use the same trick if you want on your class!

Indeed, in this example we show how an object of a  `Poly` class, meant to be a simple class storing a polygon defined by its vertices, can be initialised with the wanted vertices using braces. We show also that indeed `initializer_list<T>` may be used not just for initialization!

# What do you learn here? #
A little utility of the Standard Library, meant to mak ethe initialization of container-type structure more uniform.
