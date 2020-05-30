/*
 * ShapeVisitor.cpp
 *
 *  Created on: 03/gen/2011
 *      Author: formaggia
 */
#include <utility>
#include "ShapeVisitor.hpp"
#include "geo.hpp"

namespace Geometry2
{


void ComputeArea::visit(Quadrilateral & s)
{
  totalArea+=s.measure();
}

void CountShapes::visit(Triangle & s)
{
  ++numTriangles;
}

void CountShapes::visit(Quadrilateral & s)
{
	++numQuadrilateral;
}

void ComputeArea::visit(Point & s)
{}

void ComputeArea::visit(Triangle & s)
{
  this->totalArea+=s.measure();
}

void CountShapes::visit(Point & s)
{
  ++numPoints;
}

void swapInvertedShapes::visit(Triangle & s)
{
  if(s.measure()<0.0)
    {
      std::swap(s[0],s[1]);
      ++fixedTria;
    }
}

void swapInvertedShapes::visit(Quadrilateral & s)
{
  if(s.measure()<0.0)
    {
      std::swap(s[3],s[1]);
      ++fixedQuad;
    }
}

}//end namespace



