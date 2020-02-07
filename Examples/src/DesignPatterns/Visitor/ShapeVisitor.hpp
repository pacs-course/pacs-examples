/*
 * ShapeVisitor.hpp
 *
 *  Created on: 03/gen/2011
 *      Author: formaggia
 */

#ifndef SHAPEVISITOR_HPP_
#define SHAPEVISITOR_HPP_
namespace Geometry{
// Forward declarations

class Point;
class Triangle;
class Square;

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
  class ShapeVisitor{
  public:
    virtual void visit(Triangle&)=0;
    virtual void visit(Square&)=0;
    virtual void visit(Point&)=0;
    virtual ~ShapeVisitor()=default;
  };

  //! counts how many different shapes we have
  class CountShapes: public ShapeVisitor{
  public:
    CountShapes();
    void visit(Triangle&) override;
    void visit(Square&) override;
    void visit(Point&) override;
    int getNumPoints() const;
    int getNumSquares() const;
    int getNumTriangles() const;
  private:
    int numTriangles;
    int numSquares;
    int numPoints;
  };
  
  //! Compute and accumulate shape area
  class ComputeArea: public ShapeVisitor{
  public:
    ComputeArea();
    void visit(Triangle&) override;
    void visit(Square&) override;
    void visit(Point&) override;
    double getTotalArea() const;
  private:
    double totalArea;
  };

  //! Invert badly oriented elements
  /*! 
    This is an example of a visitor 
    that may change the state of the visited object
  */
  class swapInvertedShapes: public ShapeVisitor
  {
  public:
    swapInvertedShapes()=default;
    void visit(Triangle&) override;
    void visit(Square&) override;
    void visit(Point&) override {};// does nothing, defined in class
    double getTotalArea() const;
    int fixedTria=0;
    int fixedQuad=0;
  };
  
}
#endif /* SHAPEVISITOR_HPP_ */
