/*
 * poly.hpp
 *
 *  Created on: Feb 10, 2022
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_STL_INITIALIZERLIST_POLY_HPP_
#define EXAMPLES_SRC_STL_INITIALIZERLIST_POLY_HPP_
#include <initializer_list>
#include <vector>
namespace apsc
{
  // This is just an example of use of a parameter list as initializer
  struct Point2D
  {
    double x;
    double y;
  };

  class Poly
  {
  public:
   Poly()=default;
   Poly(std::initializer_list<Point2D> const & l):vertices{l}{};
   Poly & operator =(std::initializer_list<Point2D> const & l)
   {
     vertices=l;// inizializer lists can be assigned to vectors
     return *this;
   }
   Poly & add(std::initializer_list<Point2D> const & l)
   {
     vertices.insert(vertices.end(),l.begin(),l.end());
     return *this;
   }
   auto size() const {return vertices.size();}
   auto operator[](std::size_t i)const { return vertices[i];}
   auto & operator[](std::size_t i){ return vertices[i];}
  private:
    std::vector<Point2D> vertices;
  };
}




#endif /* EXAMPLES_SRC_STL_INITIALIZERLIST_POLY_HPP_ */
