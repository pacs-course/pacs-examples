#Decltyle and declval#

`decltype()` returns the type of an expression. But what if I want to test the type of the result of a non-static method of a class?

I need to have an object of the class to call the method. But this is not required if you just want to know the return type, since you can call the method in a "unevaluated context" using the `std::declval` magic. You can even do it on a class that is not default constructible! Or a class which has all constructor private, like in the Singleton design pattern!

