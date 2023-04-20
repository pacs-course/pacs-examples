# Classes to handle a simple one-dimensional mesh #

You need first to compile and install RKFSolver
go to `Examples/src/NewtonSolver`
do
```c++
 make -B alllibs DEBUG=no; make install
```
to install the tools needed by RKFSolver. Then

go to `Examples/src/RKFSolver` and do
```c++
make install
```
It will install the header file for the runge kutta fehlberg solver

Go back to this directory and run 
```c++
make dynamic (or static)
```
use `DEBUG=no` if you want full optimization.

In `runTestGenerator.sh` you have a scrit that runs an example of how to
launch a test.

**IF YOU WANT TO INSTALL THE CODE (needed for other examples) do**

    make distclean
    make dynamic DEBUG=no  (to disable debugging)
    make install
    make distclean

This software provides a class to store a simple 1D mesh of a one dimensional domain represented by an interval. It uses a policy, 
that derives from `OneDMeshGenerator`, to implement the actual mesh generation process. We provide two policies: one for uniform mesh generation, the other that allows to prescribe
a variable mesh spacing. The latter needs to integrate a ordinary differential equation to locate the mesh nodes; to this purpose it uses one of the  runge kutta adaptive schemes
provided in the directory `RKFSolver/`

`main_TestGenerator` is a simple application to test the sofware and `run_test_generator.sh` is a script that runs the code and lauches `gnuplot`.

# What do I learn here? #
- An example of the *strategy design pattern*, or policy. The way the mesh is generated is a policy of the class Mesh1d. According to the policy the mesh can be uniform or with a prescribed spacing;
- A way to compute the nodes of a mesh by integrating a spacing function with an ode solver. 

