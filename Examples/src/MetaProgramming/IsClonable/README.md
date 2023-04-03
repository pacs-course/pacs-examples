# Testing if a class has the `clone()` method #

This is a short but sophisticated template metaprogramming example. SFINAE at its best!  We show a use of `void_t` and `declval`.
We only test of a class has a method called `clone()` in its public interface. We do not test the return type. However we offer in a comment
how to do it if we wish to test also the return type, in particular if the return type is convertible to a `unique_prt` to the base class.

# What do you learn here? #

- A small example of metaprogramming magic. 
- A use of `true_type` and `false_type`: classes that embodied a bolean value. Useful when creating traits that have a bolean value, like in this case.
- The SFINAE in action. It the template parameter substitution in the specialised version fails it is not an error: the priamry version is used.
- The use of `void_t`. A class that embodies the type `void`. Looks pretty useless right? Well, it is instead pretty useful. It serves as a placeholder 
when you are not interested in the type returned, so you convert it to `void`, which is certainly safe since `void` is a type that means *this is not a type*. 

**Note** A similar class is present in `Utility/CloningUtilities`.