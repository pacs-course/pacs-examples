//
// Created by forma on 03/04/23.
//

#ifndef EXAMPLES_EDGEGEO_HPP
#define EXAMPLES_EDGEGEO_HPP
#include <array>
namespace apsc::Geometry
{
//! Simple class holding an edge
class EdgeGeo {
public:
  using PointContainer = std::array<double, 2>;
  // Constructor giving end points
  EdgeGeo(const PointContainer &left, const PointContainer &right)
    : M_points{left, right} {};
  //! Set end points
  void set(PointContainer &&left, PointContainer &&right) {
    M_points = {left, right};
  };

  PointContainer operator[](std::size_t i) const { return M_points[i]; }
  PointContainer& operator[](std::size_t i) { return M_points[i]; }

private:
  std::array<PointContainer, 2> M_points{{{0., 0.}, {0., 0.}}};
};

}
#endif // EXAMPLES_EDGEGEO_HPP
