/*
 * Quadrilateral.cpp
 *
 *  Created on: Apr 29, 2020
 *      Author: forma
 */
#include "FactoryTraits.hpp"
#include "Quadrilateral.hpp"
namespace Geometry
{
// ********************* QUADRILATERAL **********************

Quadrilateral::Quadrilateral() : Polygon(){};
Quadrilateral::Quadrilateral(Vertices const &v) : Polygon(v)
{
  // codice per deteminare la convessita` a n. vertici
  ;
}
Quadrilateral::Quadrilateral(Vertices const &v, bool c) : Polygon(v, c) {}

Quadrilateral::~Quadrilateral() {}

void
Quadrilateral::showMe(ostream &out) const
{
  out << "I have four sides, like any QUADRILATERAL should" << std::endl;
}

namespace
{
  /*!
   * @brief Registration helper executed during static initialization.
   *
   * When the shared library is loaded, this namespace-scope object is
   * constructed and inserts the quadrilateral builder into `polyFactory`.
   */
  struct LoadF
  {
    LoadF()
    {
      polyFactory["Quadrilateral"] = []() {
        return std::make_unique<Quadrilateral>();
      };
    }
  };
  //! Static registration object whose construction performs the registration.
  const LoadF loadf;
} // namespace
} // namespace Geometry
