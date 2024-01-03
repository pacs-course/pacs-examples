#include "PointInSimplex.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <tuple>
namespace py = pybind11;
using namespace apsc;
PYBIND11_MODULE(PointInSimplex, m)
{
    m.doc() = "tools to test if a point is inside a simplex"; // optional module docstring
    m.def("pointInTetrahedron", &pointInTetrahedron, "A function that checks if a point is inside a tetrahedron",py::arg("p"),py::arg("t"));
    m.def("pointInTriangle", &pointInTriangle, "A function that checks if a point is inside a triangle",py::arg("p"),py::arg("t"));
    m.def("pointInTriangle3", &pointInTriangle3, "A function that checks if a point is inside a triangle in 3D",py::arg("p"),
    py::arg("t"),py::arg("eps"));
};