#ifndef _TYPESWITCH_HPP__
#define _TYPESWITCH_HPP__
#include <vector>
#include <iostream>

class Point
{
public:
  using BoundaryType=void;
};

class Edge 
{
public:
  using BoundaryType=Point;  
};

class Triangle{
public:
  using BoundaryType=Edge;
};

class Tetra{
public:
  using BoundaryType=Triangle;
};

//! May be build with  T=Edges or T=Triangle or T=Tetra
template<typename GeoElement>
class Mesh{

public:
  using ElementType = GeoElement;
  using BoundaryType= typename GeoElement::BoundaryType;
  //! estracts list of boundary elements
  std::vector<BoundaryType> const &
  boundaryElementList() const
  {
    return M_boundaryElements;
  }
  // Other methods .....
 private:
  std::vector<ElementType> M_elements;
  std::vector<BoundaryType> M_boundaryElements;
};

//! A different implementation using special traits
/*! 
  Pure declaration of primary template
*/
template <int> struct IntToElement; 

//! Speciatization for dimension 0
template<>
struct IntToElement<0>{
  using ElementType=Point;
};

//! Speciatization for dimension 1
template<>
struct IntToElement<1>{
  using ElementType=Edge;
};

//! Speciatization for dimension 2
template<>
struct IntToElement<2>{
  using ElementType=Triangle;
};

//! Speciatization for dimension 3
template<>
struct IntToElement<3>{
  using ElementType=Tetra;
};

template<int N> 
class Mesh2
{
public:
  using ElementType = typename IntToElement<N>::ElementType;
  using BoundaryType= typename IntToElement<N-1>::ElementType;
  // AS BEFORE!
};


#endif
