# Finding if a point falls inside a polygon #

Given a generic polygon, we show here two functions to find if a point is falling inside the polygon.
They are just meant as prototypes. A more "professional" coding should take care of controlling floating point errors 
(important to deal with the limit cases), and treating the case when the point lies exactly on the boundary 
of the polygon, in a consistent way. Yet, the code provided here give an idea of the algorithms, and is usable.

We have implemented two types of algorithms:

1. The first one, working only for convex polygons,  calculates the dot product between the normal of each polygon edge and the vector connecting the given 
point and a point of the edge. The given point is inside only if all dot products have the same sign 
(we have not made any assumption on the orientation of the edge vertices, so we cannot assume an 
orientation for the normal).
2. The second method, widely used in computer graphics, consists in taking a half-line, here the horizontal lie 
in the halfspace `x>p[0]`, where `p[0]` indicates the `x`-coordinate of the point. If the number of intersections 
is odd, the point is inside. Some care must be taken to avoid counting twice if the intersection coincides 
with a polygon vertex. Here it has been done by using strict test only on one of the edge nodes.

The second technique is more efficient if the polygon has many edges. For small number of edges the two methods 
are quite equivalent, and the first one is somehow easier to understand.

I have created a *concept* to ensure that the user provides as polygon a class object that satisfies the following condition
* a `Poly` object must have `operator[]` that returns an `std::array<double,2>` containing 
* the coordinate of a polygon vertex.


I have made a large use of lambda expressions to enucleate the different steps necessary for the tests.

**A note:** Look also at the code in src/PointInSimplex, where you have a specialization for simplexes of 2 and 3 dimensions.


# What do I learn here?
- Two techniques used in computer graphics and also in some numerical schemes, like particle methods.
- The use of lambda expressions to separate in different units the various steps of an algorithm, making it more readable (hopefully).
- The creation of a simple concept (remember you need a C++20 compliant compiler!)
- A simple processing of user input with `getline()` and `istringstream`. 

