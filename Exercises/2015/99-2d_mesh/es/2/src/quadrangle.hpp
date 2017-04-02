/*!
  \file quadrangle.hpp
  \author Daniele A. Di Pietro <dipietro@unibg.it>
  \date 11-6-2005
*/

#ifndef QUADRANGLE_HPP
#define QUADRANGLE_HPP 1

#include "shape.hpp"

namespace Geometry {
  /**
     \class Quadrangle
     \brief A class to handle quadrangular elements
     \author Daniele A. Di Pietro <dipietro@unibg.it>
     \date 11-6-2005     
  */

  class Quadrangle : public Shape {
  public:
    /** @name Constructors 
     */
    //@{
    //! Constructor taking the ID of four points
    Quadrangle(Point P0, Point P1, Point P2, Point P3) : Shape(4, 4) {
      this->point(0) = P0;
      this->point(1) = P1;
      this->point(2) = P2;
      this->point(3) = P3;
    };
    //@}
    /** @name Methods
     */
    //@{
    //! Return the measure
    Real measure() { return 0.; };
    //! Return the \f$i^{\mathrm{th}}\f$ edge
    virtual Edge edge(int i);
    //@}
  };
}

#endif
