#An example opf python binding using pybind11#
This example shows how some function templates defined in a header file can by ported to a python module using pybind11.

## How to compile ##
The compilation is done with a standard Makefile.

## How to run ##
The python module can be imported in python as follows:
```python
import basicOptim
```
Then, you can use the functions defined in the module.

## What do I learn here? ##
- How to use pybind11 to create a python module from a C++ header file.
- The need of do explicit tempalte instantiation of the C++ templates to be exported to python.
- The richness of the pybind11 bindings: you can add explicit parameter name to the python function, you can introduce documentation.