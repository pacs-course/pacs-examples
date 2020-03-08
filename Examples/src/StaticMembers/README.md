#Example of usage of static members#

A class that stores a double, counts how many objects of that class
   have been created and is able to access the average of the value of
   all elements of that class in the program. It is an example of use
   of non constant static members to monitor something related to all
   objects of the class.
   
Often, static member of a class are const variable (or even constexpr,
constexpr implies static) that represent quantities common to all
members of the class.  For instance, a class of a `Polygon` family may
have as static member variable the number of vertices of specific
Polygons: for `Triangle` is 3, for a `Square` is 4 ...
