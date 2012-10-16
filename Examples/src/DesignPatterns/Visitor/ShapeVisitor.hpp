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

class ShapeVisitor{
public:
	virtual void visit(Triangle&);
	virtual void visit(Square&);
	virtual void visit(Point&);
	virtual ~ShapeVisitor(){};
};

class CountShapes: public ShapeVisitor{
public:
	CountShapes();
	void visit(Triangle&);
	void visit(Square&);
	void visit(Point&);
    int getNumPoints() const;
    int getNumSquares() const;
    int getNumTriangles() const;
private:
	int numTriangles;
	int numSquares;
	int numPoints;
};

class ComputeArea: public ShapeVisitor{
	public:
	ComputeArea();
	void visit(Triangle&);
	void visit(Square&);
	void visit(Point&);
    double getTotalArea() const;
	private:
	double totalArea;
};

}
#endif /* SHAPEVISITOR_HPP_ */
