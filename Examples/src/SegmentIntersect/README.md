# Finding intersections between two segments in a plane #

Finding the intersection of two segments in a plane looks easy. Indeed, it is not when you consider all possibilities:

- Segments may have a clear intersection in an internal point. This is the easy case to handle;
- Segments may intersect at the end, in various way;
- Segments may be superimposed one on the other;
- Segments may "barely intersect", but what does "barely" mean when we deal with floating point numbers, and the consequent round-off errors?
- Segments may not intersect.

Here I propose a template function

    template <class Edge_t>
    IntersectionStatus segmentIntersect(const Edge_t &, const Edge_t &,double tol)

That takes two edges of generic type `Edge_t`, where the only requirement is that `operator[int i]` on an `Edge_t` object returns the coordinates of the corresponding end stored in a `std::array<double,2>`.

`tol` is a tolerance, which is defaulted to the square root of the
machine epsilon, (approx 1.e-8). The choice of this tolerance may be
critical to sort out the limit cases.

The function returns a structure that contains an extended information
about the possible intersection. The code handles alle the possibilities mentioned above.

## Note ##

The folder contains also a Matlab version of the algorithm. Useful for comparison. 


# What do I learn here? #
- That also apparently simple computational problems require a lot of care if one wants an algorithm capable of treating all the particular cases;
- The use of an anonymous namespace to hide functions that are used only locally (the alternative is to use a namespace with a spacial name, like `internals`)
- The use of `inline` to avoid the one-definition-rule, since the header file containes the definition of non-template functions;
- An overloading of the output streaming operator to prettiprint the results;
- The use of 'block scope' to simplify the repetition of a test on different data.
- The use of a concept to specify a prerequisite on a type
