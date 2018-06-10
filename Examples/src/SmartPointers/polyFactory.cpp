#include "polyFactory.hpp"
namespace Geometry
{
  std::unique_ptr<AbstractPolygon> 
  polyFactory(Shape t){
    using std::unique_ptr;
    switch (t){
#if __cplusplus > 201103L
      //C++14
    case Shape::Triangle : return std::make_unique<Triangle>();
    case Shape::Square : return std::make_unique<Square>();
    default: return std::unique_ptr<AbstractPolygon>();
#else
    case Shape::Triangle : return std::unique_ptr<AbstractPolygon>(new Triangle);
    case Shape::Square : return std::unique_ptr<AbstractPolygon>(new Square);
    default: return std::unique_ptr<AbstractPolygon>();
#endif
    }
  } 
  
  void geometryHolder::setPolygon(Shape shape){
    // I do not need move() since the return value
    // is a rvalue (a temporary)
    this->my_poly=polyFactory(shape);
  }
  
  std::ostream &  geometryHolder::showMe(std::ostream & out){
    out<<" Geometry hoder object:"<<endl;
    if (this->my_poly==nullptr)
      cout<<"I am not storing any polygon"<<std::endl;
    else{
      cout<<"I am storing a polygon"<<std::endl;
      this->my_poly->showMe(out);
    }
    return out;
  }
}
