# Sorting and applying permutation #

It may happen that you have to sort a vector and at the same time perform the same element exchanges to other vectors. Unfortunately, there is no utility in the Standard Library that executes sorting and returns the information about the permutation carried out by the sorting.

Therefore, I have created this little utility. Things to note:

* It operates on any sequential random access containers of the standard library (not on C-style arrays), i.e. std::vector and std::array;
* The class is not default-constructible, but here is not really a problem. But with the addition of a setter
we can get round it.
* I have used an internal `struct` to store a small aggregate necessary for keeping track of the original position of an element in the container;
* I have given the possibility of selecting different comparision operators than the default one, which is `less<>` as in `std::sort`;
* Once you have sorted the main vector, you can aply the same permutation to other vectors. I have implemented bith a standard application of the permutation and an in-place algorithm 


# What do I learn here? #

- The use of lambda expressions to define a comparison operator;
- How to give a default to a template function member parameter;
- A use of internal (embedded) classes.

I have used an internal class (a struct to be precise) with private attribute since it is part of the implementation, there is no use of it outside the scope of my `SortAndPermute` class. 

In the main program you find a simple example that explains the use of this utility.

** Note: ** The in-place algorithm has been "strongly inspired" (i.e. copied) from a post of Raymond Chen: 
https://devblogs.microsoft.com/oldnewthing/20170102-00/?p=95095

** To do ** write a parallel version 