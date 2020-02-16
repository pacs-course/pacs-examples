#Example of usage of static members#

A class that stores a double, counts how many objects of that class
   have been created and is able to access the average of the value of
   all elements of that class in the program. It is an example of use
   of non constant static members to monitor something related to all
   objects of the class.
   
More often, static member of a class are const (or even constexpr,
constexpr implies static) and represent quantities common to all
members of the class. 
