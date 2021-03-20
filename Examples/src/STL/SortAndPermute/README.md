# Sorting and extract permutation #

It happens that you have to sort a vector and at the same time make the same element exchanges in other vectors.
Unfortunately, there is no utility in the Standard Library that executes sorting and returns the information about
the permutation carried out by the sorting.

Therefore, I have created this little utility. Things to note:

* It operates on any sequential random acccess containers of the standard library (not on C-style arrays), i.e. std::vector and std::array;
* It is a handler of a container: the container is passed in the constructor and stored as a reference. The sorting operates directly on the container (like in 
std::sort after all);
* As a consequence, the class is not default-constructible, but here is not really a problem;
* I have used an internal `struct` to store a small aggregate necessary for keeping track of the original position of an element in the container;
* I have given the possibility of selecting different comparision operators than the default one, which is `less<>` as in `std::sort`;
* The class is in fact a functor. 


# What do I learn here? #
- That a class can store a reference;
- The use of lambda expressions to define a comparison operator;
- How to give a default to a template function member parameter;
- A use of internal (embedded) classes.

I have used an internal class (a struct to be precise) with private attribute since it is part of the implementation, there is no use of it outside the scope of my
`SortAndPermute` class. 

In the main program you find a simple example that explains the possible use of this utility.
