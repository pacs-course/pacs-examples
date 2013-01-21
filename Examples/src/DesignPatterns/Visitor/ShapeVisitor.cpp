/*
 * ShapeVisitor.cpp
 *
 *  Created on: 03/gen/2011
 *      Author: formaggia
 */
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
this->totalArea+=s.measure();
}



void Geometry::CountShapes::visit(Triangle & s)
{
	++numTriangles;
}






Geometry::ComputeArea::ComputeArea() : totalArea(0)
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





