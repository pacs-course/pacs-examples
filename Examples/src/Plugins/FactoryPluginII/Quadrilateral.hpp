/*
 * Quadrilateral.hpp
 *
 *  Created on: Apr 29, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_FACTORYPLUGIN_QUADRILATERAL_HPP_
#define EXAMPLES_SRC_FACTORYPLUGIN_QUADRILATERAL_HPP_
#include "Polygon.hpp"
namespace Geometry
{
/*!
 * @brief Concrete polygon type representing a quadrilateral.
 *
 * In this example the class mainly serves as a plugin-registered concrete
 * product that overrides `showMe()`.
 */
class Quadrilateral : public Polygon
{
public:
  //! Builds an empty quadrilateral.
  Quadrilateral();

  //! Builds a quadrilateral from a list of vertices.
  explicit Quadrilateral(Vertices const &v);

  //! Builds a quadrilateral from vertices and an explicit convexity flag.
  explicit Quadrilateral(Vertices const &v, bool convex);

  //! Virtual destructor.
  virtual ~Quadrilateral();

  //! Prints a textual description of the quadrilateral.
  virtual void showMe(ostream &out = cout) const override;
};

} // namespace Geometry

#endif /* EXAMPLES_SRC_FACTORYPLUGIN_QUADRILATERAL_HPP_ */
