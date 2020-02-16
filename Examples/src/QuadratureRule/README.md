#Examples of a code for numerical quadrature#

* `baseversion/` Here the basic classes for numerical quadrature,
  including adaptive and Montecarlo. We use the *Decorator* design pattern to
  extend the basic rules. Note that they are all clonable classes.

* `DynamicIntegrands/` An example on how to load dynamically the
  integrand functions from a shared library. An example of dynamic loading of
  functions directly using the C style loading.

* `AllDynamic/` Here we add an object factory to be able to load also the quadrature rules dynamically!

To make things work you must

go into `../Utility` (if you have not already done so) and do

    make dynamic DEBUG=no
    make install

to install `libpacs.so` with all support utilities:

go into `../RK45` and do

    make clean
    make dynamic DEBUG=no
    make install

to install the `librk54.so` library (use `static` instead of `dynamic` if you wish to install the static library)

go into `../OneDMesh` and do

    make clean
    make dynamic DEBUG=no
    make install

to install the `libmesh.so` library

go into `../AbstractFactory` and do

    make install

to install the Object Factory utilities.

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

