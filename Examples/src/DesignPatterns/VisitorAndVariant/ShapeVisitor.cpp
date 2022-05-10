/*
 * ShapeVisitor.cpp
 *
 *  Created on: 03/gen/2011
 *      Author: formaggia
 */
#include "../VisitorAndVariant/ShapeVisitor.hpp"

#include <utility>
#include "../VisitorAndVariant/geo.hpp"

namespace Geometry2
{

  void
swapInvertedShapes::operator()(Triangle &s)
{
  if(s.measure() < 0.0)
    {
      std::swap(s[0], s[1]);
      ++fixedTria;
    }
}

void
swapInvertedShapes::operator()(Quadrilateral &s)
{
  if(s.measure() < 0.0)
    {
      std::swap(s[3], s[1]);
      ++fixedQuad;
    }
}

} // namespace Geometry2
