#include "polyFactory.hpp"
namespace Geometry
{
  std::unique_ptr<AbstractPolygon> 
  polyFactory(Shape t){
    using std::unique_ptr;
    switch (t){
#if __cplusplus > 201103L
      //C++14
    case Shape::Triangle : return make_unique<Triangle>();
    case Shape::Square : return make_unique<Square>();
    default: return unique_ptr<AbstractPolygon>();
#else
    case Shape::Triangle : return unique_ptr<AbstractPolygon>(new Triangle);
    case Shape::Square : return unique_ptr<AbstractPolygon>(new Square);
    default: return unique_ptr<AbstractPolygon>();
#endif
    }
  } 
  
  void geometryHolder::setPolygon(Shape shape){
    this->my_poly=std::move(polyFactory(shape));
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
