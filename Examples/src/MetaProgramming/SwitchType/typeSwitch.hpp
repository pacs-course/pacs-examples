#ifndef _TYPESWITCH_HPP__
#define _TYPESWITCH_HPP__
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace apsc::Geometry2
{
// Forward declarations: I need the compiler know that
// these are types even if I have not defined them yet
class Point;
class Edge;
class Triangle;
class Tetra;
/*!
Declaration of primary template
It defines a "no object"
 */
template <int> struct IntToElement : std::integral_constant<int, -1>
{
  using ElementType = void;
};

//! Speciatization for dimension 0
template <> struct IntToElement<0> : std::integral_constant<int, 0>
{
  using ElementType = Point;
};

//! Speciatization for dimension 1
template <> struct IntToElement<1> : std::integral_constant<int, 1>
{
  using ElementType = Edge;
};

//! Specialization for dimension 2
template <> struct IntToElement<2> : std::integral_constant<int, 2>
{
  using ElementType = Triangle;
};

//! Speciatization for dimension 3
template <> struct IntToElement<3> : std::integral_constant<int, 3>
{
  using ElementType = Tetra;
};

//! Now the actual classes
class Point
{
public:
  using ElementTypeID = IntToElement<0>;
  using BoundaryType = IntToElement<ElementTypeID::value - 1>::ElementType;
  using BoundaryBoundaryType =
    IntToElement<ElementTypeID::value - 2>::ElementType;
  std::ostream &
  whoAmI(std::ostream &out) const
  {
    out << " I am a Point";
    return out;
  }
};

class Edge
{
public:
  using ElementTypeID = IntToElement<1>;
  using BoundaryType = IntToElement<ElementTypeID::value - 1>::ElementType;
  using BoundaryBoundaryType =
    IntToElement<ElementTypeID::value - 2>::ElementType;
  std::ostream &
  whoAmI(std::ostream &out) const
  {
    out << " I am an Edge";
    return out;
  }
};

class Triangle
{
public:
  using ElementTypeID = IntToElement<2>;
  using BoundaryType = IntToElement<ElementTypeID::value - 1>::ElementType;
  using BoundaryBoundaryType =
    IntToElement<ElementTypeID::value - 2>::ElementType;
  std::ostream &
  whoAmI(std::ostream &out) const
  {
    out << " I am a Triangle";
    return out;
  }
};

class Tetra
{
public:
  using ElementTypeID = IntToElement<3>;
  using BoundaryType = IntToElement<ElementTypeID::value - 1>::ElementType;
  using BoundaryBoundaryType =
    IntToElement<ElementTypeID::value - 2>::ElementType;
  std::ostream &
  whoAmI(std::ostream &out) const
  {
    out << " I am a Tetra";
    return out;
  }
};

//! May be build with  1=Edges or 2=Triangle or 3=Tetra

template <int N> class Mesh
{
public:
  using ElementType = typename IntToElement<N>::ElementType;
  using BoundaryType = typename IntToElement<N - 1>::ElementType;
  void
  addBElement(BoundaryType const &e)
  {
    M_boundaryElements.push_back(e);
  }
  std::vector<BoundaryType> const &
  boundaryElementList() const
  {
    return M_boundaryElements;
  }
  // Other methods .....
  void printBoundaries(std::ostream &out) const;

private:
  std::vector<ElementType>  M_elements;
  std::vector<BoundaryType> M_boundaryElements;
};
template <int N>
void
Mesh<N>::printBoundaries(std::ostream &out) const
{
  for(auto const &b : M_boundaryElements)
    {
      b.whoAmI(std::cout);
      std::cout << std::endl;
    }
}
} // end namespace apsc::Geometry2
#endif
