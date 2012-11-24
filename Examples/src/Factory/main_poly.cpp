#include "Polygon.hpp"
#include "builders.hpp"
#include <map>
#include <string>
#include <iostream>
#include <memory>
// A very first example of factories
// The first part uses pointers to functions
// This normally goes to a separate file
typedef Geometry::Polygon* (*PolyBuilder)();
Geometry::Polygon* buildQuad(){return new Geometry::Quadrilateral;}
Geometry::Polygon* buildTria(){return new Geometry::Triangle;}
Geometry::Polygon* buildSquare(){return new Geometry::Square;}
//
int main()
{
  using namespace Geometry;
  using namespace std;
  auto_ptr<Polygon> test;
  //! the simplest form of a factory
  map<string,PolyBuilder> factory1;
  typedef map<string,PolyBuilder>::iterator Iterator;
  // Registering objects on the factory  
  factory1[string("Quadrilateral")]=&buildQuad;
  factory1[string("Triangle")]=&buildTria;
  factory1[string("Square")]=&buildSquare;
  // Try the factory
  string answer;
  while(true){
    cout<<"Quadrilateral, Triangle, Square or END?"<<endl;
    cin>>answer;
    if(answer=="END")break;
    auto where=factory1.find(answer);
    if(where != factory1.end()){
      // Second entry of where is a pointer to a builder function!
      Polygon * thePoly=(where->second)();
      thePoly->showMe(cout);
    }else{
      cout<<"ERROR: this polygon is not registered!"<<endl;
    }
  }
  
  // Now with the builder class I use unique_pointers to ensure proper
  // memory managemant of builder objects
  typedef unique_ptr<BaseBuilder<Polygon> > PolyBuilder2;
  map<string,PolyBuilder2> factory2;
  // Registering objects on the factory
  
  factory2["Quadrilateral"]=PolyBuilder2(new Builder<Quadrilateral,Polygon>),
  factory2["Triangle"]=PolyBuilder2(new Builder<Triangle,Polygon>);
  factory2["Square"]=PolyBuilder2(new Builder<Square,Polygon>);
  answer="";

  while(true){
    cout<<"Quadrilateral, Triangle, Square or END?"<<endl;
    cin>>answer;
    if(answer=="END")break;
    auto where=factory2.find(answer);
    if(where != factory2.end()){
      Polygon * thePoly=where->second->create();
      thePoly->showMe(cout);
    }else{
      cout<<"ERROR: this polygon is not registered!"<<endl;
    }
  }
}
  

