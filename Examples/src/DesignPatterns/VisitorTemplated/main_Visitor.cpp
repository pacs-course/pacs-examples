/*
 * main_Visitor.cpp
 *
 *  Created on: 03/gen/2011
 *      Author: formaggia
 */
#include <iostream>
#include <vector>
#include <memory>
#include "geo.hpp"
#include "ShapeVisitor.hpp"
#include "visitShapeCollection.hpp"
int main(){
  using namespace Geometry;
  using namespace std;
  Point p0(0.,0.);
  Point p1(1,0);
  Point p2(1,1);
  Point p3(0,1);
  Point p4(-1,0);
  vector<Point> points;
  points.push_back(p0);
  points.push_back(p1);
  points.push_back(p2);
  points.push_back(p3);
  points.push_back(p4);
  
  // Create a collection of shapes
  // I should I have used unique_ptr, but it doesn not work
  // need to check why
  using shapeHandler=std::unique_ptr<Shape>;
  vector<shapeHandler> collection;

  collection.emplace_back(new Triangle(p0,p1,p2));
  collection.emplace_back(new Triangle(p1,p3,p2)); // incorrect orientation
  collection.emplace_back(new Square(p0,p1,p2,p3));
  collection.emplace_back(new Triangle(p4,p0,p3));
  collection.emplace_back(new Triangle(p4,p0,p3));
  collection.emplace_back(&p0);
  
  // Visit the collection
  CountShapes counter;
  ComputeArea evaluateArea;
  swapInvertedShapes fixShapes;

  visitShapeCollection(collection,counter);
  cout<<"Num Points="<<counter.getNumPoints()<<endl;
  cout<<"Num Triangles="<<counter.getNumTriangles()<<endl;
  cout<<"Num Squares="<<counter.getNumSquares()<<endl;
  
  visitShapeCollection(collection,evaluateArea);
  cout<<"Area="<<evaluateArea.getTotalArea()<<endl;
  
  visitShapeCollection(collection,fixShapes);
  cout<<"Fixed triangles: "<<fixShapes.fixedTria<<std::endl;
  cout<<"Fixed squares  : "<<fixShapes.fixedQuad<<std::endl;
  
  visitShapeCollection(collection,evaluateArea);
  cout<<"Area after fix="<<evaluateArea.getTotalArea()<<endl;
 
  /*  This  does not work. Why?
      for (vector<Shape*>::iterator i=collection.begin();
      i!=collection.end();++i){
      counter.visit(**i);
      evaluateArea.visit(**i);
      }
  */
}
