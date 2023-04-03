# converting an integer to a user defined type #

This examples shows a useful trick. Sometimes you want to convert an integer constant to a type. Now, the standard library provides `std::integral_constant` that does the job, but here a wont something more specific. I want someting that depending from an integer (the dimension of the problem) returns the type of a object in a mesh.

So if the dimension is 3 I want that the boundary object of a tetrahedron be a triangle etc. 

See the code and look on hot it is done. Again, it is just an example, but it descrives a general technique that may be useful for generic programming.

# What do I learn here #
- The use of a map int->type (we use `std::integral_constant` for the purpose).
- How you can recursively define member types in a hyerarchical structure. Here we consider a mesh of simplexes.