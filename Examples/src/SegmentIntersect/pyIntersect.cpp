#include "EdgeGeo.hpp"
#include "SegmentIntersect.hpp"
#include <pybind11/pybind11.h>

#include <sstream>
namespace py = pybind11;
// some explicit template instance
using namespace apsc::Geometry;
// explicit instantiation of the templates
template class IntersectionStatus<Point>;
template class EdgeGeo<Point>;
template IntersectionStatus<Point>
segmentIntersect<EdgeGeo<Point>>(const EdgeGeo<Point> &S1,
                                 const EdgeGeo<Point> &S2, double const tol);

PYBIND11_MODULE(pyIntersect, m)
{
  // write documentation
  m.doc() = "Module for segment intersection using EdgeGeo and "
            "IntersectionStatus classes";
  // Bind Point class
  py::class_<apsc::Geometry::Point>(m, "Point")
    .def(py::init<>())
    // operator to create a Point from two doubles
    .def(py::init<double, double>())
    // operator to extract a point as an array of two doubles
    .def("__repr__",
         [](const apsc::Geometry::Point &p) {
           // Avoid std::format: it segfaults on this toolchain during repr.
           std::ostringstream oss;
           oss << "Point(" << p[0] << ", " << p[1] << ")";
           return oss.str();
         })
    .def("__len__", [](const apsc::Geometry::Point &) { return 2; })

    // getting the x coordinate and y coordinate
    .def("__getx__", [](const apsc::Geometry::Point &self) { return self[0]; })
    .def("__setx__",
         [](apsc::Geometry::Point &self, double value) { self[0] = value; })
    .def("__gety__", [](const apsc::Geometry::Point &self) { return self[1]; })
    .def("__sety__",
         [](apsc::Geometry::Point &self, double value) { self[1] = value; })
    .def("__getitem__",
         [](const apsc::Geometry::Point &self, size_t i) { return self[i]; })
    .def("__setitem__", [](apsc::Geometry::Point &self, size_t i,
                           double value) { self[i] = value; });
  // Bind EdgeGeo class
  py::class_<apsc::Geometry::EdgeGeo<apsc::Geometry::Point>>(m, "EdgeGeo")
    .def(
      py::init<const apsc::Geometry::Point &, const apsc::Geometry::Point &>(),
      py::arg("p1"), py::arg("p2"))
    .def("get_point",
         [](const apsc::Geometry::EdgeGeo<apsc::Geometry::Point> &self,
            size_t i) { return self[i]; });
  // Bind IntersectionStatus class
  py::class_<apsc::Geometry::IntersectionStatus<apsc::Geometry::Point>>(
    m, "IntersectionStatus")
    .def_readonly(
      "intersect",
      &apsc::Geometry::IntersectionStatus<apsc::Geometry::Point>::intersect)
    .def_readonly("numberOfIntersections",
                  &apsc::Geometry::IntersectionStatus<
                    apsc::Geometry::Point>::numberOfIntersections)
    .def(
      "get_intersection_point",
      [](const apsc::Geometry::IntersectionStatus<apsc::Geometry::Point> &self,
         size_t i) { return self.intersectionPoint[i]; })
    .def(
      "__repr__",
      [](const apsc::Geometry::IntersectionStatus<apsc::Geometry::Point> &self) {
        std::ostringstream oss;
        oss << "IntersectionStatus(intersect=" << self.intersect
            << ", numberOfIntersections=" << self.numberOfIntersections;
        if(self.intersect)
          {
            oss << ", points=[";
            for(unsigned int i = 0; i < self.numberOfIntersections; ++i)
              {
                if(i > 0)
                  {
                    oss << ", ";
                  }
                oss << "Point(" << self.intersectionPoint[i][0] << ", "
                    << self.intersectionPoint[i][1] << ")";
              }
            oss << "]";
          }
        oss << ", parallel=" << self.parallel
            << ", collinear=" << self.collinear
            << ", identical=" << self.identical << ")";
        return oss.str();
      })
    .def(
      "__str__",
      [](const apsc::Geometry::IntersectionStatus<apsc::Geometry::Point> &self) {
        std::ostringstream oss;
        oss << "Segment intersections:\n";
        oss << "\tSegments intersect    :" << self.intersect << "\n";
        if(self.intersect)
          {
            oss << "\tNumber of intersections:" << self.numberOfIntersections
                << "\n";
            for(unsigned int i = 0; i < self.numberOfIntersections; ++i)
              {
                oss << "\tx[" << i << "]=" << self.intersectionPoint[i][0]
                    << " y[" << i << "]=" << self.intersectionPoint[i][1]
                    << "\n";
                oss << "\tt0  =" << self.intersection_tcoor[i][0]
                    << " t1  =" << self.intersection_tcoor[i][1] << "\n";
              }
            oss << "\tSegments are identical:" << self.identical << "\n";
          }
        oss << "\tSegments are parallel :" << self.parallel << "\n";
        oss << "\tSegments are collinear:" << self.collinear << "\n";
        return oss.str();
      });
  // Bind segmentIntersect function
  m.def("segment_intersect",
        &segmentIntersect<apsc::Geometry::EdgeGeo<apsc::Geometry::Point>>,
        py::arg("S1"), py::arg("S2"),
        py::arg("tol") = std::sqrt(std::numeric_limits<double>::epsilon()));
}
