/*
 * ShapeVisitor.hpp
 *
 *  Created on: 03/gen/2011
 *      Author: formaggia
 */

#ifndef SHAPEVISITOR_HPP_
#define SHAPEVISITOR_HPP_
namespace Geometry2
{
// Forward declarations

class Point;
class Triangle;
class Quadrilateral;

//! This is the abstract class
/*!
  We rely on overloading to select the correct
  visiting method. This has drowbacks if the visited
  hierarchy (Shape in this example) has more than one level of
  inheritance, since the compiler will not be able to select
  the correct sibling when a generic Shape& is passed.

  In that situation it is better to use function with different names
  like visitTriangle(Triangle &) etc.
 */
class ShapeVisitor
{
public:
  virtual void visit(Triangle &) = 0;
  virtual void visit(Quadrilateral &) = 0;
  virtual void visit(Point &) = 0;
  virtual ~ShapeVisitor() = default;
};

//! counts how many different shapes we have
class CountShapes : public ShapeVisitor
{
public:
  void visit(Triangle &) override;
  void visit(Quadrilateral &) override;
  void visit(Point &) override;
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
class ComputeArea : public ShapeVisitor
{
public:
  void visit(Triangle &) override;
  void visit(Quadrilateral &) override;
  void visit(Point &) override;
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
class swapInvertedShapes : public ShapeVisitor
{
public:
  void visit(Triangle &) override;
  void visit(Quadrilateral &) override;
  void visit(Point &) override{}; // does nothing, defined in class
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
