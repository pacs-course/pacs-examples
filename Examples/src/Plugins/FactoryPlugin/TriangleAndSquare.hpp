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
 * Derived class for triangles
 */
class Triangle : public Polygon
{
public:
  Triangle() = default;
  explicit Triangle(Vertices const &v);
  // virtual double diameter() const;
  // virtual double inscibedCircle() const;
  void showMe(ostream &out = cout) const override;
  // qui vanno le altre funzionalita`..
};

/*!
 * Derived calss for squared
 */
class Square : public Polygon
{
public:
  Square() = default;
  explicit Square(Vertices const &v);
  // virtual double diameter() const;
  // virtual double inscibedCircle() const;
  void showMe(ostream &out = cout) const override;
  // Metodi propri del quadrato
  double diagLen() const;
};

} // namespace Geometry

#endif /* EXAMPLES_SRC_FACTORYPLUGIN_TRIANGLEANDSQUARE_HPP_ */
