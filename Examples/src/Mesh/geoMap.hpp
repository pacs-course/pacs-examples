#ifndef _HH_GEOMAP_HH_
#define _HH_GEOMAP_HH_
#include "Eigen/Dense"
#include "geo.hpp"
#include "smallMatrix.hpp"

namespace Geometry
{
template <class GeoShape> Transformation
{
private:
  SMatrix<double, GeoShape::myDim, ndim> M_J;
  SMatrix<double, GeoShape::myDim, ndim> M_invJ;
}

} // namespace Geometry
#endif
