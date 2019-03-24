#include "polyFactory.hpp"
namespace Geometry
{
  std::unique_ptr<AbstractPolygon> 
  polyFactory(Shape t){
    switch (t){
    case Shape::Triangle : return std::make_unique<Triangle>();
    case Shape::Square : return std::make_unique<Square>();
    default: return std::unique_ptr<AbstractPolygon>();
    }
  } 
  
  void geometryHolder::setPolygon(Shape shape){
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
