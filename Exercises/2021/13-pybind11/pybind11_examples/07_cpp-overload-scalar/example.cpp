#include <pybind11/pybind11.h>

#include <iostream>

// ----------------
// regular C++ code
// ----------------

double
multiply(double a, double b)
{
  std::cout << "Double" << std::endl;
  return a * b;
}

int
multiply(int a, int b)
{
  std::cout << "Int" << std::endl;
  return a * b;
}

// ----------------
// Python interface
// ----------------

namespace py = pybind11;

PYBIND11_MODULE(example, m)
{
  m.doc() = "pybind11 example plugin";

  m.def("multiply", py::overload_cast<int, int>(&multiply));
  m.def("multiply", py::overload_cast<double, double>(&multiply));
}
