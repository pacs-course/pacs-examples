/*
 * ShapeVisitor.cpp
 *
 *  Created on: 03/gen/2011
 *      Author: formaggia
 */
#include <utility>
#include "ShapeVisitor.hpp"
#include "geo.hpp"

namespace Geometry{

int CountShapes::getNumPoints() const
{
    return numPoints;
}

int CountShapes::getNumSquares() const
{
    return numSquares;
}

int CountShapes::getNumTriangles() const
{
    return numTriangles;
}

double ComputeArea::getTotalArea() const
{
    return totalArea;
}

}

void Geometry::ComputeArea::visit(Square & s)
{
  totalArea+=s.measure();
}



void Geometry::CountShapes::visit(Triangle & s)
{
  ++numTriangles;
}






Geometry::ComputeArea::ComputeArea() : totalArea(0.0)
{}




void Geometry::CountShapes::visit(Square & s)
{
	++numSquares;
}



Geometry::CountShapes::CountShapes():numTriangles(0),numSquares(0),numPoints(0)
{
}



void Geometry::ComputeArea::visit(Point & s)
{}



void Geometry::ComputeArea::visit(Triangle & s)
{
	this->totalArea+=s.measure();
}



void Geometry::CountShapes::visit(Point & s)
{
	++numPoints;
}

void Geometry::swapInvertedShapes::visit(Triangle & s)
{
  if(s.measure()<0.0)
    {
      auto & points=s.getPoints();
      std::swap(points[0],points[1]);
      ++fixedTria;
    }
}

void Geometry::swapInvertedShapes::visit(Square & s)
{
  if(s.measure()<0.0)
    {
      auto & points=s.getPoints();
      std::swap(points[3],points[1]);
      ++fixedQuad;
    }
}




