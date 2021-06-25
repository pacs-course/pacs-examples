#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

#include <Eigen/LU>

// N.B. this would equally work with Eigen-types that are not predefined. For
// example replacing all occurrences of "Eigen::MatrixXd" with "MatD", with the
// following definition:
//
// using MatD = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic,
//                            Eigen::RowMajor>;

// ----------------
// regular C++ code
// ----------------

Eigen::MatrixXd
inv(const Eigen::MatrixXd &xs)
{
  return xs.inverse();
}

double
det(const Eigen::MatrixXd &xs)
{
  return xs.determinant();
}

// ----------------
// Python interface
// ----------------

namespace py = pybind11;

PYBIND11_MODULE(example, m)
{
  m.doc() = "pybind11 example plugin";

  m.def("inv", &inv);

  m.def("det", &det);
}
