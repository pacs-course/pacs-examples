#include "basicOptimization.hpp" // Include the header file where the functions are defined
#include "pybind11/functional.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace py = pybind11;
using namespace apsc;
/*
I need to explicitely instantiate the templates for the functions I want to
export to python.
*/
using funct = const std::function<double(double)> &;
template std::tuple<double, bool>
apsc::golden_search(funct f, double a, double b, double tol, unsigned maxIter);
template std::array<double, 2> apsc::Brent_glomin(double a, double b, double x0,
                                                  double m, double e, double t,
                                                  funct f);
template std::array<double, 2> apsc::Brent_local_min(double a, double b,
                                                     double tol, funct f);
template std::tuple<double, double, int>
apsc::bracketIntervalMinimum(funct f, double x1, double h, unsigned maxIter);

PYBIND11_MODULE(basicOptim, m)
{
    m.def("golden_search", &golden_search<funct>),
    py::arg("f"),py::arg("a"),py::arg("b"),py::arg("tol"),py::arg("maxIter"), 
    R"pbdoc(
        Golden search function.

        This function performs golden search optimization to find the minimum of a given function.

        Args:
            f (function): The function to be minimized.
            a (float): The lower bound of the search interval.
            b (float): The upper bound of the search interval.
            tol (float): The tolerance for convergence.
            maxIter (int): The maximum number of iterations.

        Returns:
            tuple: A tuple containing the minimum value and a boolean indicating if the optimization converged.

        )pbdoc");

    m.def("Brent_glomin", &Brent_glomin<funct>, py::arg("a"), py::arg("b"),
          py::arg("x0"), py::arg("m"), py::arg("e"), py::arg("t"), py::arg("f"),
          R"pbdoc(
        Brent glomin function.

        This function performs Brent's method for global optimization to find the minimum of a given function.

        Args:
            a (float): The lower bound of the search interval.
            b (float): The upper bound of the search interval.
            x0 (float): The initial guess for the minimum.
            m (float): The tolerance for convergence.
            e (float): The tolerance for convergence.
            t (float): The tolerance for convergence.
            f (function): The function to be minimized.

        Returns:
            array: An array containing the minimum value and the number of iterations performed.

        )pbdoc");

    m.def("Brent_local_min", &Brent_local_min<funct>, py::arg("a"),
          py::arg("b"), py::arg("tol"), py::arg("f"),
          R"pbdoc(
        Brent local min function.

        This function performs Brent's method for local optimization to find the minimum of a given function.

        Args:
            a (float): The lower bound of the search interval.
            b (float): The upper bound of the search interval.
            tol (float): The tolerance for convergence.
            f (function): The function to be minimized.

        Returns:
            array: An array containing the minimum value and the number of iterations performed.

        )pbdoc");

    m.def("bracketIntervalMinimum", &bracketIntervalMinimum<funct>,
          py::arg("f"), py::arg("x1"), py::arg("h"), py::arg("maxIter"),
          R"pbdoc(
        Bracket interval minimum function.

        This function finds an interval containing the minimum of a given function.

        Args:
            f (function): The function to be minimized.
            x1 (float): The initial guess for the minimum.
            h (float): The step size for bracketing.
            maxIter (int): The maximum number of iterations.

        Returns:
            tuple: A tuple containing the minimum value, the minimum value's derivative, and the number of iterations performed.

        )pbdoc");
}
