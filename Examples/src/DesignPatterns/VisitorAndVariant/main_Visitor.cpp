/*
 * main_Visitor.cpp
 *
 *  Created on: 03/gen/2011
 *      Author: formaggia
 */
#include "overloaded.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <variant>

#include "../VisitorAndVariant/geo.hpp"
#include "../VisitorAndVariant/ShapeVisitor.hpp"
template <class C, class V>
void visitShapeCollection(C & c, V & v)
{
  for (auto & p : c)
    std::visit(v,p);
}

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
  using shapeHandler = std::variant<Point,Triangle,Quadrilateral>;
  vector<shapeHandler> collection;

  collection.emplace_back(Triangle{p0, p1, p2});
  collection.emplace_back(Triangle{p1, p3, p2}); // incorrect orientation
  collection.emplace_back(Quadrilateral{p0, p1, p2, p3});
  collection.emplace_back(Triangle{p4, p0, p3});
  collection.emplace_back(Triangle{p4, p0, p3});
  collection.emplace_back(Point{0., 0.});

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

  // Now I show how to create a visitor on the fly! Another version of countShapes
  std::vector<unsigned int> counter2={0u,0u,0u};
  auto countAllShapes = apsc::overloaded {
    [&counter2](Point const & ){++counter2[0];},
    [&counter2](Triangle const & ){++counter2[1];},
    [&counter2](Quadrilateral const & ){++counter2[2];},
  };
  visitShapeCollection(collection, countAllShapes);
  cout << "Num Points=" << counter2[0] << endl;
  cout << "Num Triangles=" << counter2[1] << endl;
  cout << "Num Squares=" << counter2[2] << endl;
}
