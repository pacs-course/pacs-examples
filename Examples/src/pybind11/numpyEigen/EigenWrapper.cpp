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
  // Use LU factorization
  return xs.partialPivLu().solve(ys);
  // Use QR factorization
  // return xs.colPivHouseholderQr().solve(ys);
}

using Vector = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using Vectori = Eigen::Matrix<int, Eigen::Dynamic, 1>;
// Here an example of overloaded function
/*!
 *
 * @param xs The matrix
 * @param fac The vector to multipy with
 * @return The new matrix
 */
Vector
multiply(const Eigen::MatrixXd &xs, const Vector &fac)
{
  return xs * fac;
}
/*!
 * THhe overload for integer matrices
 * @param xs the matrix
 * @param fac the vector to muliply with
 * @return The new matrix
 */
Vectori
multiply(const Eigen::MatrixXi &xs, Vectori const &fac)
{
  return xs * fac;
}

// ----------------
// Python interface
// ----------------

// complete the following pybind11 wrapper for the functions defined above
PYBIND11_MODULE(eigenwrapper, m)
{
  namespace py = pybind11;
  m.doc() = "pybind11 example of a wrapper for Eigen matrices";

  m.def("inv", &inv, "A function that computes the inverse of a matrix");

  m.def("det", &det, "A function that computes the determinant of a matrix");

  m.def("solve", &solve, "A function that solves a linear system");
  // N.B. the order here is crucial, in the reversed order every "int" is
  // converted to a "double"
  m.def(
    "multiply",
    py::overload_cast<const Eigen::MatrixXi &, const Vectori &>(&multiply),
    "A function that multipliesEigen::MatrixXd a integer matrix by an integer");
  m.def("multiply",
        py::overload_cast<const Eigen::MatrixXd &, const Vector &>(&multiply),
        "A function that multiplies a matrix by a double");
}
