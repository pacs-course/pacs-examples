# An example of policy # 

We want to define a class, called `interp1D`
that parform the interpolation of a vector of values given the couple
`X`,`Y` of interpolation nodes and values. The type of interpolation is
implemented via a policy, that, given `X` and `Y` provides the method to
interpolate a single value at a given location `x`.

I have implemented polynomial Lagrange interpolation, using the classical
formula, which interpolates on the polynomial of degree n-1, being n
the number of interpolation nodes, Lagrange interpolation using the
Newton formula (which is slightly more stable) and piecewise linear
interpolation.

You can add other interpolators, if you wish! For instance cubic splines! Why not!

*Note* I have here chosen to have a set of unrelated classes, which however share part of their public interface.
You may instead want to create a hierarchy of interpolators by cretin a base class that exposes the common public interface. 
This will provide greater flexibility (at a little loss of efficiency to  the call of virtual methods.)

## Parallel algorithm ##
In the code you may choose to activate a parallel algorithm, to have parallel computing for free. Remember that in that case you need to link with the tbb library, look at the `Makefile.inc` file. 


# What do I learn here ? #
- Another use of a policy to provide a component that changes the behaviour of an algorithm.

