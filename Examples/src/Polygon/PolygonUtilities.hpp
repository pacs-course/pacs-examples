#ifndef HH_POLYGONUTILITIES_HH
#define HH_POLYGONUTILITIES_HH
#include "Polygon.hpp"
#include <memory>
#include <string>
namespace Geometry
{
/*!
 * @fn std::unique_ptr<AbstractPolygon> createPolygon(const std::string&,
 * Args...)
 * @brief A simple factory of polygons
 *
 * @tparam Args The type of the optional arguments
 * @param name The name of the polygon
 * @param args The optional arguments to be passed to the constructor
 * @return A pointer to the abstract class pointing to the newly created
 * concrete Polygon
 */
template <typename... Args>
std::unique_ptr<AbstractPolygon>
createPolygon(std::string const &name, const Args &...args)
{
  if(name == "Polygon")
    return std::make_unique<Polygon>(args...);
  if(name == "Square")
    return std::make_unique<Square>(args...);
  if(name == "Triangle")
    return std::make_unique<Triangle>(args...);
  // this way I can easily check if I try to construct a non existent Polygon
  return std::unique_ptr<AbstractPolygon>{nullptr};
}
} // namespace Geometry
#endif
