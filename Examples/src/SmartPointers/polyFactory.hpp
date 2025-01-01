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
  This enumerator is not implicitely convertible to an int
  (it is explicitely convertible instead)
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
  This simple version uses the defualt ocnstructor to constuct the
  objects of the polymprphic family. Less flexible than the more complex
  version below.

  Indeed, you need to use setters afterwards if you need to change the state from the
  default one.

  @note I have chosen to inline it, so to keep everything in the header file
*/
inline
std::unique_ptr<AbstractPolygon>
polyFactory_simple(Shape s)
{
  switch(s)
    {
    case Shape::Triangle:
      return std::make_unique<Triangle>();
    case Shape::Square:
      return std::make_unique<Square>();
    case Shape::Polygon:
      return std::make_unique<Polygon>();
    default:
      return nullptr;
    }
}



//! A factory of polygons.
/*!
  This is a less simple simple example of object factory.
  With the help of variadic templates I can use any constructor of the polymorphic object.
  Indeed,  different Polygon objects have
  different constructors, taking different arguments.

  Here I use forwarding references to forward to the contractor the argument category value (move what can be moved
  copy what should be copied). But I present also (commented) a simpler version.
*/
template <class... Args>
std::unique_ptr<AbstractPolygon>
polyFactory(Shape s, Args&&... args)// simpler version:polyFactory(Shape s, const Args&... args)
{
  switch(s)
    {
    case Shape::Triangle:
      // simpler version
      //return std::make_unique<Triangle>(args...);
        return std::make_unique<Triangle>(std::forward<Args>(args)...);
    case Shape::Square:
        return std::make_unique<Square>(std::forward<Args>(args)...);
    case Shape::Polygon:
        return std::make_unique<Polygon>(std::forward<Args>(args)...);
    default:
        return nullptr;
    }
}

} // namespace Geometry
#endif
