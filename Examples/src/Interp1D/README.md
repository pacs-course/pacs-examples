# A generic piecewise linear interpolator#

In this directory we show an example of a generic piecewise linear 1D interpolator, and one application for
vectors of couples of values, representing the key (the interpolation nodes) and values (the interopolation values).

It is rather generic and accepts in input iterators to any container with bi-directional iterators. Bi-directionality
is indeed only needed if we do extrapolation on the right. In principle it could be checked out by using iterator_tags.

Anyway, it is better to use it with containers that have a randomaccess iterators (like vector or array) because only in this case
we have optimal complexity (log2 n).

