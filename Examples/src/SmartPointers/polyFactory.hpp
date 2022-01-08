#ifndef _HH_POLYFACTORY_HH__
#define _HH_POLYFACTORY_HH__
#include "Polygon.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <utility>
namespace Geometry
{
//! To drive the factory
/*!
  This is an enum class. Introduced with C++11
  This enumerator is not convertible to an int.
  It has to be addressed with the qualified name:
  Shape::Triangle of Shape::Square
*/
enum class Shape
{
  Polygon,
  Triangle,
  Square
};
//! A factory of polygons.
/*!
  This is a not so simle simple example of object factory. For simplicity I use
  a switch statement on an enum.
  The complexity comes from the fact that different Polygon objects have
  different constructors, taking different arguments. So I need to use variadic
  templates! and if constexpr.
*/
template <Shape SHAPE, class... Args>
std::unique_ptr<AbstractPolygon>
polyFactory(Args&&... args)
{
  if constexpr(SHAPE == Shape::Triangle)
    return std::make_unique<Triangle>(std::forward<Args>(args)...);
  else if constexpr(SHAPE == Shape::Square)
    return std::make_unique<Square>(std::forward<Args>(args)...);
  else if constexpr(SHAPE == Shape::Polygon)
    return std::make_unique<Polygon>()(std::forward<Args>(args)...);
  else
    return nullptr; // To avoid compiler complains
}

} // namespace Geometry
#endif
