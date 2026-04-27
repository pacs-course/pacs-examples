/*
 * TraiangleAndSquare.cpp
 *
 *  Created on: Apr 29, 2020
 *      Author: forma
 */
#include "FactoryTraits.hpp"
#include "TriangleAndSquare.hpp"
namespace Geometry
{
// ********************* TRIANGLE **********************

Triangle::Triangle(Vertices const &v) : Polygon(v, true) {}

void
Triangle::showMe(ostream &out) const
{
  out << "The eternal TRIANGLE" << std::endl;
}

// ********************* SQUARE **********************

Square::Square(Vertices const &v) : Polygon(v, true) {}

void
Square::showMe(ostream &out) const
{
  out << "I am rather SQUARE, what about you?" << std::endl;
}

double
Square::diagLen() const
{
  // not implemented yet
  return 0;
};

namespace
{
  /*!
   * @brief Registration helper executed during static initialization.
   *
   * When the shared library is loaded, this namespace-scope object is
   * constructed and inserts the triangle and square builders into
   * `polyFactory`.
   */
  struct LoadF
  {
    LoadF()
    {
      polyFactory["Triangle"] = []() { return std::make_unique<Triangle>(); };
      polyFactory["Square"] = []() { return std::make_unique<Square>(); };
    }
  };
  //! Static registration object whose construction performs the registration.
  const LoadF loadf;
} // namespace
} // namespace Geometry
