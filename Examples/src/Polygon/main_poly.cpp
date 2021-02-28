#include "Polygon.hpp"
#include "PolygonUtilities.hpp"
#include <iostream>
//! Main program
int main()
{
  using namespace Geometry;

  //! five vertices
  Vertices v(5);
  v[0]={0.5,0.5};
  v[1]={0.5,0.8};
  v[2]={0.0,0.0};
  v[3]={0.0,-1.0};
  v[4]={0.3,-0.5};

  Polygon aPolygon(v);
  aPolygon.showMe();
  std::cout<<"Area: "<<aPolygon.area()<<std::endl;
  // A triangle built with the first 3 vertices
  Triangle aTriangle(Vertices(v.begin(),v.begin()+3));
  // testing polymorphism
  AbstractPolygon * p_ab=&aTriangle;

  p_ab->showMe();
  std::cout<<"Area: "<<aTriangle.area()<<std::endl;
  //! Unit Square
  Square aSquare({0.0,0.0},1.0);
  Square s2(aSquare); 
  AbstractPolygon & r_ab=s2;
  r_ab.showMe();
  std::cout<<"Area: "<<r_ab.area()<<std::endl;

  // You can also do like this
  Triangle pippo{Vertices{{0.,1},{1.,0.},{0.,0.}}};
  std::cout<<"Area of unitary triangle: "<<pippo.area()<<std::endl;
  std::cout<<"Triangle vertices: "<<std::endl;
  for (auto k: pippo)std::cout<<k;
  
  // testing the factory
  auto ppoly = createPolygon("Polygon");
  ppoly->setVertexes(v);
  ppoly->showMe();
  std::cout<<"Area: "<<ppoly->area()<<std::endl;
  auto anotherSquare = createPolygon("Square");
  Vertices q{{0,0},{0,1},{1,1},{1,0}};
  anotherSquare->setVertexes(q);
  anotherSquare->showMe();
  std::cout<<"Area: "<<anotherSquare->area()<<std::endl;
  
}
  

