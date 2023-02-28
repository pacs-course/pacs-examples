# Utilities to read and store a mesh of triangles #

A set of class and utilities that can be used to reas and store a
triangular mesh.

Some notable things:

* Classes that define geometric entities in `geo.hpp`
* Classes to extend the goemetric entitie to become FInite Element entities, in `femMesh.hpp`
* Classes that define tools to read a mesh form files in different formats, in ``meshReaders.hpp``
* A trick (with the use of `friend`) to make private data available to the functions that read the mesh without exposing them to the general public.
* The triangular mesh in `MeshTria.hpp`
* A class to store small matrices, thought for holding finite element local matrices, with a set of useful operators. In `smallMatrix.hpp`
    
## Testing the code ##
In the directory `Test` the files to produce a mesh on a triangle or of the letter A using the mesh generator [Triangle](https://www.cs.cmu.edu/~quake/triangle.html).
YOu must have  [Triangle](https://www.cs.cmu.edu/~quake/triangle.html) installed. Triangle is a nice and simple 2D mesh generator. You should have it available (on request) 
in your Linux distribution. If not, get it from [this site](https://netlib.org/voronoi/index.html), download it, compile it and place it in 
your `bin` directory (or in `/usr/local/bin`). There is also a program, called `showMe` to visualize the mesh produced by triangle.

Documentation is on the page indicated, and a lot of stuff (save it in a file!) is given by typing `triangle -h`.

To produce the meshes needed by the test progrem you have to go in the `Test` folder and type
- `bash generateMeshA.sh` to generate a triangular mesh and 
- `bash generateMeshQuad.sh` to generate a quad mesh.

## Compilation ##
- `make dynamic` to produce dynamic library and an executable with a simple test
- `make static` to produce static library and a code to run a test.

## Using the code in other examples ##
To use the utilities here provided and collected in the library `libMesh.so` (or `libMesh.a` if you prefere static libraries)
you need to do 

```
make dynamic (or static)
make install
```

# What do I learn here 
- A nice use of `friend`liness
- Some use of composition by inheritance (buld more complec classes by ineriting from simple components)
- A simple class to handle triangular and quad meshes.
