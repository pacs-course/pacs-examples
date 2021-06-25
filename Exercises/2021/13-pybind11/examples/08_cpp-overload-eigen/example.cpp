#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

#include <iostream>

#include <Eigen/LU>

// ----------------
// regular C++ code
// ----------------

Eigen::MatrixXd
multiply(const Eigen::MatrixXd &xs, double fac)
{
  std::cout << "Double" << std::endl;
  return fac * xs;
}

Eigen::MatrixXi
multiply(const Eigen::MatrixXi &xs, int fac)
{
  std::cout << "Int" << std::endl;
  return fac * xs;
}

// ----------------
// Python interface
// ----------------

namespace py = pybind11;

PYBIND11_MODULE(example, m)
{
  m.doc() = "pybind11 example plugin";

  // N.B. the order here is crucial, in the reversed order every "int" is
  // converted to a "double"
  m.def("multiply", py::overload_cast<const Eigen::MatrixXi &, int>(&multiply));
  m.def("multiply",
        py::overload_cast<const Eigen::MatrixXd &, double>(&multiply));
}
