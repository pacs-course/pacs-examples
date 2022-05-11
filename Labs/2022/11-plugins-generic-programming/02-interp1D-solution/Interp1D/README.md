# A generic piecewise linear interpolator#

In this directory we show a rather complex example of a generic piecewise linear 1D interpolator, and one application for
vectors of couples of values, representing the key (the interpolation nodes) and values (the interpolation values).

It is rather generic and accepts in input iterators to any container with bi-directional iterators. Bi-directionality
is indeed only needed if we do extrapolation on the right. In principle it could be checked out by using iterator_tags.

Anyway, it is better to use it with containers that have a randomaccess iterators (like vector or array) because only in this case we have optimal complexity (log2 n).

The generality comes from the fact that I wanted a function that takes in input sn object from which I can extract keys and values and I want also to specify the comparison operator among keys, in a general way. This is in `interp1D.hpp`.

Then I have written two oveloads that represent the more common usage of the tool.  A first overload that takes a vector of elements that have the operator [i] returning the key  (the interpolation nodes) for i=0 and the value (the interpolation point)  for i=1, for instance
`std::vector<std::array<double,2>>` and an overload that takes a vector of keys and a vector of values. The second overload is very similar to the interp1D function of Matlab.

For the second overload I need to implement some adapters to obey the interface of the generic function. It is a complex, and a little convoluted, example. 

#What do I learn from this examples?#
- The use of generic programming to develop a flexible tool, capable of handling different ways the input data is structured
- The use of specializations, via function overloading, can provide the user with a simpler iterface for the most common data structures.
- The use of adapters to interface a code with data structures apparently incompatible with the given interface.
- How function overloading may be used to provide different interfece to a common utility. Here `interp1D` may be called in different "versions", depending on the type of data available. 

