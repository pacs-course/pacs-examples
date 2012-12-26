#ifndef _TYPESWITCH_HPP__
#define _TYPESWITCH_HPP__
#include <vector>
#include <iostream>
//! We can use the type to switch between implementations
/*! Is an alternative to template specialization
 */
class Edge {
};

class Triangle{
public:
  typedef Edge BoundaryType;
};

class Tetra{
public:
  typedef Triangle BoundaryType;
};

//A simple empty struct that allows to select types
template<typename T>
struct TypeToType{};

//! May be build with  T=Triangle or T=Tetra
template<typename GeoElement>
class mesh{

public:
  typedef typename GeoElement::BoundaryType BoundaryType;
  //! estracts list of boundary elements
  /*! The correct list is selected using the type
   */
  std::vector<BoundaryType> &
  boundaryElementList(){ return 
      _boundaryElementList(TypeToType<BoundaryType>());
      }

 private:
  std::vector<Edge> _edges;
  std::vector<Triangle> _triangles;
  std::vector<Tetra> _tetras;
  std::vector<Edge>& _boundaryElementList(TypeToType<Edge>){
    std::cout<<"Returning Edges"<<std::endl;
    return _edges;}
  std::vector<Triangle>& _boundaryElementList(TypeToType<Triangle>){
    std::cout<<"Returning Triangles"<<std::endl;
    return _triangles;}
};
//
// Of course as an alternative I can use template specialization
/*
template<>
vector<Edge> Mesh<Triangle>::boundaryElementList(){
  return this->_edges;
}

template<>
vector<Triangle> Mesh<Tetra>::boundaryElementList(){
  return this->_tetras;
}
*/

//! A different implementation using special traits

template <int T> struct
IntToElement;
template<>
struct IntToElement<1>{
  typedef Edge ElementType;
};
template<>
struct IntToElement<2>{
  typedef Triangle ElementType;
};
template<>
struct IntToElement<3>{
  typedef Tetra ElementType;
};

template<int T> class 
Mesh2{
  typedef typename IntToElement<T>::ElementType ElementType;
  typedef typename IntToElement<T-1>::ElementType BElementType;
};


#endif
