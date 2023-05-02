//
// Created by forma on 04/04/23.
//
#include <string>
#include <vector>
#include "pybind11/stl.h"
#include "pybind11/pybind11.h"
#include "pybind11/functional.h"
#include "mesh.hpp"
namespace py=pybind11;
using namespace Geometry;
/*!
 * \brief Python binding for the class OneDMeshGenerator
 * \ingroup meshers
 */
class PyMeshGenerator:public  OneDMeshGenerator
{
public:
  using OneDMeshGenerator::OneDMeshGenerator;
  MeshNodes operator()() const override
  {
    PYBIND11_OVERLOAD_PURE_NAME(
        MeshNodes, /* Return type */
        OneDMeshGenerator, /* Parent class */
        "__call__", /* Name of function in Python */
        operator(), /* Name of function in C++ (must match Python name) */
        /* Argument(s) */
        );
  }
};

/*!
 *  Wrapper for the utilities in the Examples/src/OneDMesh folder
 */
PYBIND11_MODULE(OneDMesh, m) {
  m.doc()="A simple one dimensional mesh generator";
  py::class_<Domain1D>(m,"Domain1D")
    .def(py::init<double const &,double const &>(),py::arg("left"),py::arg("right"))
    .def("left",py::overload_cast<>(&Domain1D::left))
    .def("left",py::overload_cast<>(&Domain1D::left,py::const_))
    .def("right",py::overload_cast<>(&Domain1D::right))
    .def("right",py::overload_cast<>(&Domain1D::right,py::const_))
    .def("__repr__",
        [](const Domain1D &a) {
            return "<OneDMesh.domain with ends " + std::to_string(a.left()) +
                    " and " + std::to_string(a.right()) + ">";
        }
    );
  py::class_<OneDMeshGenerator,PyMeshGenerator>(m,"OneDMeshGenerator")
    .def(py::init<Domain1D const &>(),py::arg("domain"))
    .def("getDomain",&OneDMeshGenerator::getDomain)
    .def("__call__",&OneDMeshGenerator::operator())
    .def("__repr__",
        [](const OneDMeshGenerator &a) {
            return "<OneDMesh.meshGenerator with domain " + std::to_string(a.getDomain().left()) +
                    " and " + std::to_string(a.getDomain().right()) + ">";
        }
    );
  py::class_<Uniform,OneDMeshGenerator>(m,"Uniform")
    .def(py::init<Domain1D const &,int const &>(),py::arg("domain"),py::arg("nNodes")=100)
    .def("__repr__",
        [](const Uniform &a) {
            return "<OneDMesh.Uniform with domain " + std::to_string(a.getDomain().left()) +
                    " and " + std::to_string(a.getDomain().right()) + ">";
        }
    );
  py::class_<VariableSize,OneDMeshGenerator>(m,"VariableSize")
    .def(py::init<Domain1D const &,std::function<double(double)> const &,
      unsigned const int>(),py::arg("domain"),py::arg("sizeFunction"),py::arg("nNodes")=100)
    .def("__repr__",
        [](const VariableSize &a) {
            return "<OneDMesh.VariableSize with domain " + std::to_string(a.getDomain().left()) +
                    " and " + std::to_string(a.getDomain().right())  +  ">";
        }
    );
  py::class_<Mesh1D>(m,"Mesh1D")
    .def(py::init<>())
    .def(py::init<Domain1D const &,unsigned int const &>(),py::arg("domain"),py::arg("nNodes")=100)
    .def(py::init<OneDMeshGenerator const &>(),py::arg("meshGenerator"))
    .def("reset",&Mesh1D::reset)
    .def("numNodes",&Mesh1D::numNodes)
    .def("__getitem__",&Mesh1D::operator[],py::arg("i"))
    .def("__len__",&Mesh1D::numNodes)
    .def("domain",&Mesh1D::domain)
    .def("hmin",&Mesh1D::hmin)
    .def("hmax",&Mesh1D::hmax)
    .def("__iter__",
        [](const Mesh1D &a) {
            return py::make_iterator(a.cbegin(), a.cend());
        },
        py::keep_alive<0, 1>()) /* Essential: keep object alive while iterator exists */
    .def("meshNodes",[](const Mesh1D &a) {return std::vector<double>{a.cbegin(),a.cend()};})
    .def("__repr__",
        [](const Mesh1D &a) {
            return "<OneDMesh.mesh with " + std::to_string(a.numNodes()) +
                    " nodes>";
        }
    );
}



