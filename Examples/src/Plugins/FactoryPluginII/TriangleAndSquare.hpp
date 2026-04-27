/*
 * TriangleAndSquare.hpp
 *
 *  Created on: Apr 29, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_FACTORYPLUGIN_TRIANGLEANDSQUARE_HPP_
#define EXAMPLES_SRC_FACTORYPLUGIN_TRIANGLEANDSQUARE_HPP_
#include "Polygon.hpp"
namespace Geometry
{
/*!
 * @brief Concrete polygon type representing a triangle.
 */
class Triangle : public Polygon
{
public:
  //! Builds an empty triangle.
  Triangle() = default;

  //! Builds a triangle from its vertices.
  explicit Triangle(Vertices const &v);

  //! Prints a textual description of the triangle.
  void showMe(ostream &out = cout) const override;
};

/*!
 * @brief Concrete polygon type representing a square.
 */
class Square : public Polygon
{
public:
  //! Builds an empty square.
  Square() = default;

  //! Builds a square from its vertices.
  explicit Square(Vertices const &v);

  //! Prints a textual description of the square.
  void showMe(ostream &out = cout) const override;

  //! Placeholder for the diagonal-length computation.
  double diagLen() const;
};

} // namespace Geometry

#endif /* EXAMPLES_SRC_FACTORYPLUGIN_TRIANGLEANDSQUARE_HPP_ */
