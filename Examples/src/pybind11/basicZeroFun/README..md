# basic routines for finding the zero of a function #

This examples shows how existing function templates, more precisely the ones in `LinearAlgebraUtilities/basicZeroFun.hpp`, can be used in Python. The function templates are wrapped using pybind11.

The file in `LinearAlgebraUtilities/basicZeroFun.hpp` must be copied (installed) in Examples/include for this example to work. 

We need to do an explicit instantiation of the template functions since we need to provide pybind11 wrapper with concrete objects, not templates (we have no templates in python).

For the rest, the code is rather simple. You need to provide the function with the correct return type, and the correct argument types. The rest is done by pybind11.

In python we can now do:

```python
import zeroFun as zero
def fun(x) -> float:
    return (x-1.)**3
solution=zero.Newton(fun,dfun,0.,1.e-8,1.e-12,100)
```
Note that I have prescribed the return type of the python function. It is not strictly necessary, but it is good practice. It avoids python to derive a wrong return type via duck-typing. For instance if you do

```python
def fun(x):
    return (x-1.)**3.5
```
python will deduce `complex` as return type, to allow the function to be valid also for negative values of `x`. This is not what we want, and we can avoid it by explicitly specifying the return type.

# What do I learn here? #
- How to wrap existing function templates
- How to use explicit template instantiation
- How to exploit existing C++ code in python
- How to specify the return type of a python function