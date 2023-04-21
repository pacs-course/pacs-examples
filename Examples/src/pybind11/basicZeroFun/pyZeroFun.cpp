#include <functional>
#include "pybind11/pybind11.h"
#include "pybind11/functional.h"
#include "basicZeroFun.hpp"

// you need to instantiate the templates
using funct=const std::function<double (double)> &;

template double apsc::regulaFalsi(funct f, double a, double b, double tol, double tola);
template double apsc::bisection(funct f, double a, double b, double tol);
template std::tuple<double, bool> apsc::secant(funct f, double a, double b, double tol, double tola,unsigned int maxit);
template std::tuple<double, bool> apsc::Newton(funct f, funct df, double a, double tol, double tola,unsigned int maxit);
template std::tuple<double, double, bool> apsc::bracketInterval(funct f, double x1, double h,unsigned int maxIter);
template std::tuple<double, bool> apsc::brent_search(funct f, double a, double b, double tol,unsigned int maxit);

/*!
python binder.

Yiu can make it better by adding descriptors
 */
PYBIND11_MODULE(zeroFun, m) {
    using namespace apsc;
    m.def("regulaFalsi", &regulaFalsi<funct>);
    m.def("bisection", &bisection<funct>);
    m.def("secant", &secant<funct>);
    m.def("Newton",&Newton<funct,funct>);
    m.def("bracketInterval",&bracketInterval<funct>);
    m.def("brent_search",&brent_search<funct>);
};

