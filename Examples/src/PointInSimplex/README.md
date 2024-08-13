# Locate if a point is insidede a 2D of 3D simplex #
We provide a set of utilities to verify if a point is inside a 2D or 3D simplex in the following situations:
- The simplex is a triangle in the 2D space (a 2-simplex).
- The simplex is a tetrahedron in the 3D space (a 3-simplex).
- The simplex is a triangle in the 3D space (a 2-simplex in 3D).

The code is self explanatory. We have implemented the possibility of using the Eigen library (the default) of use standard containers for the simple structures holding
points and simplices.

The actual code is just in a headed file `PointInSimplex.h`. The file `pointInSimplex.cpp` is just a bider for a python module 

You can compile the code with the following commands:

```bash
make all
```
compiles the cpp test code and the python module
    
```bash
make pyModule
```
compiles just the python module
    
```bash
make main_PIS
```
compiles just the cpp test code

The python code `pyPIS_test.py` is a simple test of the python module. You can run it with

```bash
python pyPIS_test.py
```

## What do I learn here?
- The use of Eigen library for linear algebra.
- The use of traits to define the type of the container holding the points and the simplices and the use of adaptors to be able to use semilessly eigen martices or standard containers
- The use of pybind11 to create a python module.