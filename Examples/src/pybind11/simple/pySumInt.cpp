#include <pybind11/pybind11.h>
#include "sumInt.hpp"
PYBIND11_MODULE(mymodule, m) {
  namespace py=pybind11;
    m.doc() = "This module is an example of pybind11 usage"; // optional module docstring

    /* The part with the arguments (py::arg) may be omitted but it is nice 
       since the python functin can now be called also indicating the 
       arguments: sumInt(m=10,n=20)
    */
    m.def("sumInt", &sumInt, "A function that sums integers between n and m",
          py::arg("m"),py::arg("n"));
}
