# An example of policy via templates #

We want to be able to compare strings using either case sensitive 
or case insensitive equality comparison.

To that purpose we create two policies in the termof functors that compare two characters. I use the standard function `std::toupper`, which converts a char to its corresponding upper case, to construct the policy for case insensitive comparison. In `CompareString.hpp` I show how the policies can be passed as tempalte argument to either change the behaviour of a function template, or modify the state of a cunfor that compares strings.


As an additional too, I have implemented here a possible hash function for `std::string` when you ignore case in comparison. Using that hash function, in an unordered container strings differring only on the case of corresponding characters are treated as equivalent.


# What do I learn here? #

- A simple use of policy
- How to build a hash function.
