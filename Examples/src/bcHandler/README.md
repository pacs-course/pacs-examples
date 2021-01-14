# A class for boundary conditions #

An example of a class that handles boundary conditions. But in fact is an example on the possible use of global namespace variables and of an expresion parser (muparserX).

To run the example you should have `muparserX` installed locally. To do so, go into `Example/Extras/muparserx` and launch the script `install_PACS.sh`. 
It should be enough.

The idea is that when solving a differential probelms you have to handle boundary conditions (which normally are more an implementation headache than the differential equation). The idea here is to have a class that encapsulate the concept of a boundary condition. We identify a boundary condition by the following attributes

- An optional descriptor, useful to remember what the bc is about. For instance "top left side" of "inflow section".
- A type that allows to identify the type of boundary condition. Typically Dirichlet, Neumann etc. Since the application will proebably have to treat different types differently.
- The identifier of the entities where the bc is applied. An entity could be a point, a side, a face, or the identifier of a set of points/sides/faces. The actual meaning of the identifier will be secified by the actual application. For what concerns our class it is just a list of integers.
- A function that returns the associated value at the boundary. Now, here we may have different possibility. I have chosen for this class of boundary condition that the function associated is a function of a scalar variable t (for time) and a vector of double (for the space coordinates).

For the function, here I have used to wrap them in a std::function. This gives great flexibility, including, as you may see in the code, mixing user defined funcion written in C++ with expression parsed from a file, using [muparserX](https://beltoforion.de/en/muparserx/). (Remember that using an expression parser is very convenient, but has a considerable computational overhead, it's up to you to decide if the loss of efficiency is justified by the greater flexibility).

Some hints on the general architecture. In `bcType_traits.hpp` you
have the definition of the main types and some utilities used
throughout the code. Here I have not used a struct to encapsulate the
traits, I just put all the definitions in the sub-namespace `FEM`.

I recall that using the traits technique help in ensuring type
consistency acress your program, help readability, and allow to change
some types throughout your application more easily. For instance, I
may decite that the entiti identifier should not be `unsigned int` but
an `int`, and I have only to change the definition of the type alias
`Id`.

The boundary condition type is stored internally as an
enum. But it is nice to have also the "human readable" equivalent. To
pass from one to the other I am using an `std::map` global variable
(in fact it is an `inline` variable), that will be included in all the
translation units of my little program,

To handle muparserX expression I have created a muparser interface
class, which is just a variation of tha ontained in the
`muParserInterface` folder.

The main reads from a simple text file the list of boudnary
conditions I want to apply. 

Ok, you can look by yourself the rest. The code is heavily
commented. 

