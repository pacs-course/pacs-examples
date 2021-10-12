/*
 * main_Visitor.cpp
 *
 *  Created on: 03/gen/2011
 *      Author: formaggia
 */
#include "ShapeVisitor.hpp"
#include "geo.hpp"
#include "visitShapeCollection.hpp"
#include <iostream>
#include <memory>
#include <vector>
int
main()
{
  using namespace Geometry2;
  using namespace std;
  Point p0(0., 0.);
  Point p1(1, 0);
  Point p2(1, 1);
  Point p3(0, 1);
  Point p4(-1, 0);

  // Create a collection of shapes
  // Here I use just a vector of pointer (unique)
  using shapeHandler = std::unique_ptr<Shape>;
  vector<shapeHandler> collection;

  collection.emplace_back(std::make_unique<Triangle>(p0, p1, p2));
  collection.emplace_back(
    std::make_unique<Triangle>(p1, p3, p2)); // incorrect orientation
  collection.emplace_back(std::make_unique<Quadrilateral>(p0, p1, p2, p3));
  collection.emplace_back(std::make_unique<Triangle>(p4, p0, p3));
  collection.emplace_back(std::make_unique<Triangle>(p4, p0, p3));
  collection.emplace_back(std::make_unique<Point>(0., 0.));

  // Visitor for the the collection
  CountShapes        counter;
  ComputeArea        evaluateArea;
  swapInvertedShapes fixShapes;

  visitShapeCollection(collection, counter);
  cout << "Num Points=" << counter.getNumPoints() << endl;
  cout << "Num Triangles=" << counter.getNumTriangles() << endl;
  cout << "Num Squares=" << counter.getNumSquares() << endl;

  visitShapeCollection(collection, evaluateArea);
  cout << "Area=" << evaluateArea.getTotalArea() << endl;

  visitShapeCollection(collection, fixShapes);
  cout << "Fixed triangles: " << fixShapes.numFixedTria() << std::endl;
  cout << "Fixed squares  : " << fixShapes.numFixedQuad() << std::endl;

  visitShapeCollection(collection, evaluateArea);
  cout << "Area after fix=" << evaluateArea.getTotalArea() << endl;

  /*  This  does not work. Why?
      for (vector<Shape*>::iterator i=collection.begin();
      i!=collection.end();++i){
      counter.visit(**i);
      evaluateArea.visit(**i);
      }
  */
}
