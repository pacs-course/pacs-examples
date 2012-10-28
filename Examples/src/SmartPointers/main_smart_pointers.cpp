#include "polyFactory.hpp"
#include <iostream>
//! An example of smart pointers.
/*!
  @note beware. With clang 3.1 compiled over g++ 4.6 there is a problem of
  incompatibility of the standard library shipped with g++ 4.6!
 */
int main(){
  using namespace Geometry;
  using namespace std;
  geometryHolder a;
  a.showMe();
  // Assign a triangle
  a.setPolygon(polyFactory(Shape::Triangle));
  a.showMe();
  // Triangle is destroyed, now I own a Square
  a.setPolygon(polyFactory(Shape::Square));
  a.showMe();

  // This works only of compiled with g++ or with clang with newest version 
  // of the standard library (that shipped with gcc 4.7)
  // I have a patch
  std::shared_ptr<AbstractPolygon> apoly(polyFactory(Shape::Triangle).release());
  std::shared_ptr<AbstractPolygon> bpoly(apoly);
  cout<<"there are "<<bpoly.use_count()<<" shared pointers owning a polygon"<<endl;
  apoly.reset(); // apoly is now empty
  cout<<"there are "<<bpoly.use_count()<<" shared pointers owning a polygon"<<endl;
  bpoly.reset(); // bpoly is now empty. Polygon is deleted
  cout<<"there are "<<bpoly.use_count()<<" shared pointers owning a polygon"<<endl;
}
