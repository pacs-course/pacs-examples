#An example of use of global variables and local functions.#

Usually in C++ varaibles have "internal linkage" that is they are local to the translation unit, or scope, where they are defined, while function have "external linkage", thait is they are accessible by all translation units that link with the code where they are defined. This is normally what you expect and want. 

But sometimes you may want to "break the rule" and have either variables with exteernal linkage, normally calles **global variables** or function with internal linkage, sometimes called **local functions**. 

For instance `std::cout` is a *global variable*, in the namespace `std` so it may be called a *namespace variable*, declared in the header file `iostream` and defined somewhere in the standard library. All codes that include `iostream` may use it, and indeed we know that it is of type `std::ostream` and is by default connected to the terminal. But its definition is not in our code, is in the standard library.

You should try to avoid global variables, but sometimes they are useful, if not necessary, to make sure that the variable is consistently used acreoss different translation units. For instance, you may wish to have a global variable containing the main parameters of your code. Or you may want to use an object factory, in that case the factory must be a global variable: all pieces of your code should have access to the same factory.

What's instead the use of local functions? Well, sometimes you create a function that is meant to be  used only locally in the source code you are writing. It is not meant to be at disposal of all the translation units that compose your code. So, to reduce the risk of name clashes, you make them local.

In the simple code in this directory we recall the ways in which in C++ we can create global variables or local function. Have a look.

#What do I learn here?#
- A first example of use of global variables (variables with global linkage)
- A first example of use of local functions (functions with local linkage)
 
