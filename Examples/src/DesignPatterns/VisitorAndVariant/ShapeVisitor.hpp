/*
 * ShapeVisitor.hpp
 *
 *  Created on: 03/gen/2011
 *      Author: formaggia
 */

#ifndef SHAPEVISITOR_HPP_
#define SHAPEVISITOR_HPP_
#include "../VisitorAndVariant/geo.hpp"

namespace Geometry2
{
// Forward declarations

//! counts how many different shapes we have
class CountShapes
{
public:
  void
  operator()(Triangle const &)
  {
    ++numTriangles;
  };
  void
  operator()(Quadrilateral const &)
  {
    ++numQuadrilateral;
  };
  void operator()(Point const &){++numPoints;}
  int
  getNumPoints() const
  {
    return numPoints;
  }
  int
  getNumSquares() const
  {
    return numQuadrilateral;
  }
  int
  getNumTriangles() const
  {
    return numTriangles;
  }

private:
  int numTriangles = 0;
  int numQuadrilateral = 0;
  int numPoints = 0;
};

//! Compute and accumulate shape area
class ComputeArea
{
public:
  void
  operator()(Shape const &p)
  {
    totalArea += p.measure();
  };
  double
  getTotalArea() const
  {
    return totalArea;
  }

private:
  double totalArea = 0.0;
};

//! Invert badly oriented elements
/*!
  This is an example of a visitor
  that may change the state of the visited object
*/
class swapInvertedShapes
{
public:
  void operator()(Triangle &);
  void operator()(Quadrilateral &);
  void operator()(Point &){}; // does nothing, defined in class
  int
  numFixedTria() const
  {
    return fixedTria;
  }
  int
  numFixedQuad() const
  {
    return fixedQuad;
  }

private:
  int fixedTria = 0;
  int fixedQuad = 0;
};

} // namespace Geometry2
#endif /* SHAPEVISITOR_HPP_ */
