#ifndef HH_POLYGONUTILITIES_HH
#define HH_POLYGONUTILITIES_HH
#include <string>
#include <memory>
#include "Polygon.hpp"
namespace Geometry
{

  /*!
   * A simple factory of polygons that construct an element of the AbstractPolygon hierarchy using
   * the default constructor
   * @param name The name of the polygon
   * @return A poligon
   */
std::unique_ptr<AbstractPolygon> createPolygon(std::string const & name)
{
  if(name=="Polygon")
    return std::make_unique<Polygon>();
  if(name=="Square")
    return std::make_unique<Square>();
  if(name=="Triangle")
    return std::make_unique<Triangle>();
  // this way I can check if I try to construct a non existent Polygon
  return std::unique_ptr<AbstractPolygon>(nullptr);
}
}

#endif
