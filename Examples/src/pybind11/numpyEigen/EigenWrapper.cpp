//
// Created by forma on 21/04/23.
//
#include <Eigen/Dense>
#include <Eigen/LU>
#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

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

Eigen::VectorXd
solve(const Eigen::MatrixXd &xs, const Eigen::VectorXd &ys)
{
  return xs.colPivHouseholderQr().solve(ys);
}

// Here an example of overloaded function
/*!
 *
 * @param xs The matrix
 * @param fac The factor to multiply with
 * @return The new matrix
 */
Eigen::MatrixXd
multiply(const Eigen::MatrixXd &xs, double fac)
{
  return fac * xs;
}
/*!
 * THhe overload for integer matrices
 * @param xs the matrix
 * @param fac the factor to muliply with
 * @return The new matrix
 */
Eigen::MatrixXi
multiply(const Eigen::MatrixXi &xs, int fac)
{
  return fac * xs;
}


// ----------------
// Python interface
// ----------------


PYBIND11_MODULE(eigenwrapper,m)
{
  namespace py = pybind11;
  m.doc() = "pybind11 example plugin";

  m.def("inv", &inv,"A function that computes the inverse of a matrix");

  m.def("det", &det,"A function that computes the determinant of a matrix");

  m.def("solve", &solve,"A function that solves a linear system");
 // N.B. the order here is crucial, in the reversed order every "int" is
  // converted to a "double"
  m.def("multiply", py::overload_cast<const Eigen::MatrixXi &, int>(&multiply),
        "A function that multiplies a integer matrix by an integer");
  m.def("multiply",
        py::overload_cast<const Eigen::MatrixXd &, double>(&multiply),
          "A function that multiplies a matrix by a double");
}
