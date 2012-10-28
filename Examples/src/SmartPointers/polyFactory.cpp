#include "polyFactory.hpp"
namespace Geometry
{
  std::unique_ptr<AbstractPolygon> 
  polyFactory(Shape t){
    using std::unique_ptr;
    switch (t){
    case Shape::Triangle : return unique_ptr<AbstractPolygon>(new Triangle);
    case Shape::Square : return unique_ptr<AbstractPolygon>(new Square);
    default: return unique_ptr<AbstractPolygon>();
    }
  } 
  
  void geometryHolder::setPolygon(std::unique_ptr<AbstractPolygon> p){
    this->my_poly=std::move(p);
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
