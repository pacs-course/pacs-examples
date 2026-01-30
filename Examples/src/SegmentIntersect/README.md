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

That takes two edges of generic type `Edge_t`, where the only requirement is that `operator[int i]` on an `Edge_t` object returns the coordinates of the corresponding end stored in an object that behaves like an array of doubles.

`tol` is a tolerance, which is defaulted to the square root of the
machine epsilon, (approx 1.e-8). The choice of this tolerance may be
critical to sort out the limit cases.

The function returns a structure that contains an extended information
about the possible intersection. The code handles alle the possibilities mentioned above.

## Note ##



# What do I learn here? #
- That also apparently simple computational problems require a lot of care if one wants an algorithm capable of treating all the particular cases;
- The use nested namespaces
- An overloading of the output streaming operator to prettiprint the results;
- The use of 'block scope' to simplify the repetition of a test on different data.
- The use of a concept to specify a prerequisite on a type
- The binding with pybind11

## Folder contents ##

- `SegmentIntersect.hpp`: core implementation of the segment intersection algorithm and the `IntersectionStatus` result struct.
- `EdgeGeo.hpp`: lightweight geometric primitives (`Point` as `std::array<double,2>` and `EdgeGeo`) plus concepts for type requirements.
- `main_intersect.cpp`: C++ demo program that exercises the intersection routine.
- `pyIntersect.cpp`: pybind11 bindings exposing `Point`, `EdgeGeo`, and `segment_intersect` to Python.
- `segmentIntersect.py`: small Python CLI script that prompts for two segments and prints the intersection result.
- `Makefile`: build targets for the C++ example and the Python extension module.
- `pyIntersect.cpython-*.so`: built Python extension module (generated artifact).
- `main_intersect`: compiled C++ executable (generated artifact).

# The instructions to compile: #

```bash
make
```
for the c++ executable

```bash
make python
```
for the python module.
