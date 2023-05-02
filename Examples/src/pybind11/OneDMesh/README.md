# Interfacing with a more complex class #

Here we show how to create a python wrapper for the functionalities present in the folder `src/OneDMesh`. So, first thing you have to go in that folder and make sure that 
the code is **installed** using **dynamic libraries**, as explained in the `README.md` file in that folder.

Then, you have to go in the folder `src/pybind11/OneDMesh` and do

    make all
to compile the python module. 

Then, you can run the test script `pyMesh1d.py` to see if everything works.

The main complexities here is to treat pure virtual methods, call operators, overloaded methods, iterating over a mesh object, and so on.
Refer to the pybind11 documentation for detailed explanation.

## What do I learn here? ##
- How to wrap a class with pure virtual methods
- How to wrap a class with overloaded methods
- How to wrap a class with a call operator
- How to wrap a class with iterators

