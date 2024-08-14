#include "basicZeroFun.hpp"
#include "pybind11/functional.h"
#include "pybind11/pybind11.h"
#include <functional>

// you need to instantiate the templates
using funct = const std::function<double(double)> &;
using functl = const std::function<long double(long double)> &;

template double apsc::regulaFalsi(funct f, double a, double b, double tol,
                                  double tola);
template double apsc::bisection(funct f, double a, double b, double tol);
template double apsc::bisection(functl f, double a, double b, double tol);
template std::tuple<double, bool> apsc::secant(funct f, double a, double b,
                                               double tol, double tola,
                                               unsigned int maxit);
template std::tuple<double, bool> apsc::Newton(funct f, funct df, double a,
                                               double tol, double tola,
                                               unsigned int maxit);
template std::tuple<double, double, bool>
apsc::bracketInterval(funct f, double x1, double h, unsigned int maxIter);
template std::tuple<double, bool>
apsc::brent_search(funct f, double a, double b, double tol, unsigned int maxit);

/*!
python binder.

Yiu can make it better by adding descriptors
 */
PYBIND11_MODULE(zeroFun, m)
{
  using namespace apsc;
  namespace py = pybind11;
  // Bind the regulaFalsi function
  m.def("regulaFalsi", &regulaFalsi<funct>, py::arg("f"), py::arg("a"),
        py::arg("b"), py::arg("tol"), py::arg("tola"),
        R"pbdoc(
                        Find the zero of a function using the regula falsi method.

                        :param f: The function to find the zero of.
                        :param a: The lower bound of the interval.
                        :param b: The upper bound of the interval.
                        :param tol: The tolerance for the zero.
                        :param tola: The tolerance for the function value.
                        :return: The zero of the function.
                    )pbdoc");

  // Bind the bisection function
  m.def("bisectionld", &bisection<functl>, py::arg("f"), py::arg("a"),
        py::arg("b"), py::arg("tol"),
        R"pbdoc(
                        Find the zero of a function using the bisection method.

                        :param f: The function to find the zero of.
                        :param a: The lower bound of the interval.
                        :param b: The upper bound of the interval.
                        :param tol: The tolerance for the zero.
                        :return: The zero of the function.
                    )pbdoc");
  m.def("bisection", &bisection<funct>, py::arg("f"), py::arg("a"),
        py::arg("b"), py::arg("tol"),
        R"pbdoc(
                        Find the zero of a function using the bisection method.

                        :param f: The function to find the zero of.
                        :param a: The lower bound of the interval.
                        :param b: The upper bound of the interval.
                        :param tol: The tolerance for the zero.
                        :return: The zero of the function.
                    )pbdoc");

  // Bind the secant function
  m.def("secant", &secant<funct>, py::arg("f"), py::arg("a"), py::arg("b"),
        py::arg("tol"), py::arg("tola"), py::arg("maxit"),
        R"pbdoc(
                        Find the zero of a function using the secant method.

                        :param f: The function to find the zero of.
                        :param a: The first initial guess.
                        :param b: The second initial guess.
                        :param tol: The tolerance for the zero.
                        :param tola: The tolerance for the function value.
                        :param maxit: The maximum number of iterations.
                        :return: A tuple containing the zero of the function and a boolean indicating convergence.
                    )pbdoc");

  // Bind the Newton function
  m.def("Newton", &Newton<funct, funct>, py::arg("f"), py::arg("df"),
        py::arg("a"), py::arg("tol"), py::arg("tola"), py::arg("maxit"),
        R"pbdoc(
                        Find the zero of a function using the Newton method.

                        :param f: The function to find the zero of.
                        :param df: The derivative of the function.
                        :param a: The initial guess.
                        :param tol: The tolerance for the zero.
                        :param tola: The tolerance for the function value.
                        :param maxit: The maximum number of iterations.
                        :return: A tuple containing the zero of the function, a boolean indicating convergence, and the number of iterations.
                    )pbdoc");

  // Bind the bracketInterval function
  m.def("bracketInterval", &bracketInterval<funct>, py::arg("f"), py::arg("x1"),
        py::arg("h"), py::arg("maxIter"),
        R"pbdoc(
                        Find an interval that brackets a zero of a function.

                        :param f: The function to find the zero of.
                        :param x1: The initial guess.
                        :param h: The step size.
                        :param maxIter: The maximum number of iterations.
                        :return: A tuple containing the lower and upper bounds of the bracketed interval, and a boolean indicating convergence.
                    )pbdoc");

  // Bind the brent_search function
  m.def("brent_search", &brent_search<funct>, py::arg("f"), py::arg("a"),
        py::arg("b"), py::arg("tol"), py::arg("maxit"),
        R"pbdoc(
                        Find the zero of a function using Brent's method.

                        :param f: The function to find the zero of.
                        :param a: The lower bound of the interval.
                        :param b: The upper bound of the interval.
                        :param tol: The tolerance for the zero.
                        :param maxit: The maximum number of iterations.
                        :return: A tuple containing the zero of the function and a boolean indicating convergence.
                    )pbdoc");

  m.doc() = "A set of utilities to find the zero of a scalar function";
};
