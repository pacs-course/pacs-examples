# Examples of a code for numerical quadrature #

* `baseversion/` Here the basic classes for numerical quadrature,
  including adaptive and Montecarlo. We use the *Decorator* design pattern to
  extend the basic rules. Note that they are all clonable classes.

* `AllDynamic/` Here we add an object factory to be able to load also
  the quadrature rules dynamically. We use the Generic Factory in the
  folder `GenericFactory`, replicated in `Utility/`. The integrands
  are also loaded dynamically but with the trick of using C style
  linkage so that in the library the functiosn are stored with their
  name, not mangled.  Clearly, it is also possible to use a factory to
  load the functions, using a similar structure used for the rules.
  The integrand is wrapped in a std::function, so in principle it is
  possible to use a parser like muparser, as seen in other examples,
  with a loss in efficiency (but a gain in flexibility).

To make things work you must

go into `../Utility` (if you have not already done so) and do

    make dynamic DEBUG=no
    make install

to install `libpacs.so` with all support utilities:

go into `../NewtonSolver` and do
```bash
make dynamic DEBUG=no
make install
```
go into `../RKFSolver` and do

    make install

go into `../OneDMesh` and do

    make clean
    make dynamic DEBUG=no
    make install

to install the `libmesh.so` library

Then in `baseVersion/`

    make dynamic| static 
  
  or

    make alllibs
    
followed by

    make install

Then,

    make exec 
    
makes a test the uses only the basis version.

Similarly in the other directories. 

**In** `AllDynamic\` **you can only use dynamic libraries. So make sure to run** 
```bash
make dynamic; 
make install`
```
or 
```bash
make allibs; 
make install`
```

 **before using the code in** `AllDynamic`


