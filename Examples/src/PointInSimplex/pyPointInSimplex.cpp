#include "PointInSimplex.hpp"
#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <tuple>

namespace py = pybind11;
using namespace apsc;

PYBIND11_MODULE(PointInSimplex, m)
{
  m.doc() =
    "tools to test if a point is inside a simplex"; // optional module docstring
  m.def("pointInTetrahedron", &pointInTetrahedron,
        "A function that checks if a point is inside a tetrahedron",
        py::arg("p"), py::arg("t"));
  m.def("pointInTriangle", &pointInTriangle,
        "A function that checks if a point is inside a triangle", py::arg("p"),
        py::arg("t"));
  m.def("pointInTriangle3", &pointInTriangle3,
        "A function that checks if a point is inside a triangle in 3D",
        py::arg("p"), py::arg("t"), py::arg("eps") = 1e-3);
  // Python-friendly wrappers: build and return the simplex.
  // Binding the variadic template directly is brittle with pybind11.
  m.def(
    "make_simplex",
    [](const PointInS_Traits::Point3D &p1, const PointInS_Traits::Point3D &p2,
       const PointInS_Traits::Point3D &p3, const PointInS_Traits::Point3D &p4)
    {
      PointInS_Traits::Tetrahedron simplex;
      make_simplex(simplex, p1, p2, p3, p4);
      return simplex;
    },
    "Create a tetrahedron simplex from four 3D points", py::arg("p1"),
    py::arg("p2"), py::arg("p3"), py::arg("p4"));

  m.def(
    "make_simplex",
    [](const PointInS_Traits::Point2D &p1, const PointInS_Traits::Point2D &p2,
       const PointInS_Traits::Point2D &p3)
    {
      PointInS_Traits::Triangle simplex;
      make_simplex(simplex, p1, p2, p3);
      return simplex;
    },
    "Create a 2D triangle simplex from three 2D points", py::arg("p1"),
    py::arg("p2"), py::arg("p3"));

  m.def(
    "make_simplex",
    [](const PointInS_Traits::Point3D &p1, const PointInS_Traits::Point3D &p2,
       const PointInS_Traits::Point3D &p3)
    {
      PointInS_Traits::Triangle3D simplex;
      make_simplex(simplex, p1, p2, p3);
      return simplex;
    },
    "Create a 3D triangle simplex from three 3D points", py::arg("p1"),
    py::arg("p2"), py::arg("p3"));

  // Backward-compatible aliases.
  m.def(
    "make_simplex_tetrahedron",
    [](const PointInS_Traits::Point3D &p1, const PointInS_Traits::Point3D &p2,
       const PointInS_Traits::Point3D &p3, const PointInS_Traits::Point3D &p4)
    {
      PointInS_Traits::Tetrahedron simplex;
      make_simplex(simplex, p1, p2, p3, p4);
      return simplex;
    },
    "Alias of make_simplex for a tetrahedron", py::arg("p1"), py::arg("p2"),
    py::arg("p3"), py::arg("p4"));
  m.def(
    "make_simplex_triangle",
    [](const PointInS_Traits::Point2D &p1, const PointInS_Traits::Point2D &p2,
       const PointInS_Traits::Point2D &p3)
    {
      PointInS_Traits::Triangle simplex;
      make_simplex(simplex, p1, p2, p3);
      return simplex;
    },
    "Alias of make_simplex for a 2D triangle", py::arg("p1"), py::arg("p2"),
    py::arg("p3"));
  m.def(
    "make_simplex_triangle3D",
    [](const PointInS_Traits::Point3D &p1, const PointInS_Traits::Point3D &p2,
       const PointInS_Traits::Point3D &p3)
    {
      PointInS_Traits::Triangle3D simplex;
      make_simplex(simplex, p1, p2, p3);
      return simplex;
    },
    "Alias of make_simplex for a 3D triangle", py::arg("p1"), py::arg("p2"),
    py::arg("p3"));
};
