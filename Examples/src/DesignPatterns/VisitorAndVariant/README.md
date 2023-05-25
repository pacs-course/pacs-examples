# The Visitor Design Pattern with Variant #

Here an example of using visitors on variants. It is a very elegant solution for having
a double dispatch (even if we do not explit here polymorphism with the visitors). 

A bit less scalable than the "classic" version, Indeed if the number of classes to be handled by the visitor grows, using variant becomes cumbersome. 

But is a nice example of the use of varaints and of the `std::visit` utility.

I also take the occasion of show the use of the `apsc::overloaded` utility contained in the `Utility` folder. **Remember to install it by running `make install` in the `Utility` folder!**.

#What do I learn here?#

- A use of `std::variant` and of variadic templates (in `apsc::overloaded`).
- The `std::visit` utility.
