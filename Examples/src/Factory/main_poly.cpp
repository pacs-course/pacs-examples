#include "Polygon.hpp"
#include "builders.hpp"
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <functional>
// A very first example of factories

// This normally goes to a separate file
using PolyBuilder=std::function<std::unique_ptr<Geometry::Polygon>()>;
//
int main()
{
  using namespace Geometry;
  using namespace std;
  //! the simplest form of a factory
  map<string,PolyBuilder> factory1;
  // Registering builders on the factory
  // I use lambdas since they are practical
  factory1[string("Quadrilateral")]=[](){return std::unique_ptr<Polygon>(new Geometry::Quadrilateral);};
  factory1[string("Triangle")]=[](){return std::unique_ptr<Polygon>(new Geometry::Triangle);};
  factory1[string("Square")]=[](){return std::unique_ptr<Polygon>(new Geometry::Square);};
  // Try the factory
  while(true){
  string answer;
  cout<<"Quadrilateral, Triangle, Square or END?"<<endl;
  cin>>answer;
    if(answer=="END")break;
    auto where=factory1.find(answer);
    if(where != factory1.end()){
      // Second entry of where is a pointer to a builder function!
      auto thePoly=(where->second)();
      thePoly->showMe(cout);
    }else{
      cout<<"ERROR: this polygon is not registered!"<<endl;
    }
    // WHO IS DELETING THE POLY OBJECT???
  }

  
  // Now with the builder class I use unique_pointers to ensure proper
  // memory managemant of builder objects
  typedef unique_ptr<BaseBuilder<Polygon> > PolyBuilder2;
  map<string,PolyBuilder2> factory2;
  // Registering objects on the factory
  
  factory2["Quadrilateral"]=PolyBuilder2(new Builder<Quadrilateral,Polygon>),
  factory2["Triangle"]=PolyBuilder2(new Builder<Triangle,Polygon>);
  factory2["Square"]=PolyBuilder2(new Builder<Square,Polygon>);

  while(true){
    string answer;
    cout<<"Quadrilateral, Triangle, Square or END?"<<endl;
    cin>>answer;
    if(answer=="END")break;
    auto where=factory2.find(answer);
    if(where != factory2.end()){
      auto thePoly=where->second->create();
      thePoly->showMe(cout);
    }else{
      cout<<"ERROR: this polygon is not registered!"<<endl;
    }
  }
}
  

