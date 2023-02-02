# A template class for derivatives of a function by finite differences #

This is an example of template (meta)programming to have a class that implements derivatives of any order of a function
approximating them by finite differencing. It is an example of recursion. In practice the N-th derivative of f is approximated
as the 1st derivative of the Nth-1 derivative, and so on recursively until we get to the 1st derivative, where I use a standard two point formula.

To get a more stable (and accurate) formula for higher order derivatives I alternate backward and
forward differences, but you can also use centered formula. Howver, beware that the standard three point formula for the second derivative is obtained
by chosing the backward or forward alternative, not the centered ones. With the algorithm I have implemented a centered second order derivative will use a stencil of five points, not three.

In this example I also show a use or template lambda for the same purpose.  Instead of using a template class, here I use a template (automatic) lambda expression! Clearly I had to decide wich finite difference to use (centered in this case), but the code is very neat! And the use of if constexpr allows closing the recursion without need of specialisation (but you can do specialization if you prefer).

** Note ** The code in this folder may be used by other examples. Do `make install` to install it in the right folder.

**What does this example show?**

- An example of use of recursion with templates;
- The (c++17 extension) `if constexpr` construct, to select a conditioned branch **at compile time**. Without `if constexpr` we could have resorted to a metaprogramming technique called `tag dispatching` , see [here](www.fluentcpp.com/2018/04/27/tag-dispatching/). I have used tag dispatching in my `MyMat0` class for matrices to select the storage ordering;
- We also show how to make helper functions to create specific instances and simplify the use of the tool.
- We also show a nice use of lambda expression, derived from a code in *"Discovering Modern C++", P. Gottschiling, Addison Wesley, 2016. and II edition 2021*
