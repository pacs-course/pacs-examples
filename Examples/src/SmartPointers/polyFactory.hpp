#ifndef _HH_POLYFACTORY_HH__
#define _HH_POLYFACTORY_HH__
#include <memory>
#include <string>
#include <iostream>
#include "Polygon.hpp"
namespace Geometry
{
  //! To drive the factory
  enum class Shape {Triangle,Square};
  //! A factory of polygons.
  /*!
    This is a simple example of object factory. For simplicity I use
    a switch statement on a string.
  */
  std::unique_ptr<AbstractPolygon> polyFactory(Shape t);
  
  //! An example of composition via unique_ptr.
  /*! This class owns a AbstractPolygon using a unique_ptr
    The class is pretty useless, it is here only to show
    how unique_ptrs work;
  */
  class geometryHolder{
  public:
    geometryHolder()=default;
    geometryHolder(std::unique_ptr<AbstractPolygon> p):my_poly(std::move(p)){};
    //! transfer ownership to the class
    void setPolygon(std::unique_ptr<AbstractPolygon> p);
    ostream & showMe(std::ostream & out=std::cout);
  private:
    std::unique_ptr<AbstractPolygon> my_poly; 
  };
}
#endif
