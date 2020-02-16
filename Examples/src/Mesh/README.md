# Utilities to read and store a mesh of triangles#

A set of class and utilities that can be used to reas and store a
triangular mesh.

Some notable things


* Classes that define geometric entities in `geo.hpp`
* Classes to extend the goemetric entitie to become FInite Element entities, in `femMesh.hpp`
* Classes that define tools to read a mesh form files in different formats, in ``meshReaders.hpp``
* The triangular mesh in `MeshTria.hpp`
* A class to store small matrices, though for holding finite element local matrices, with a set of useful operators. In `smallMatrix.hpp`
    
In the directory `Test` the files to produce a mesh on a triangle or of the letter A using the mesh generator [Triangle](https://www.cs.cmu.edu/~quake/triangle.html).
