/*!
  \file triangle.hpp
  \author Daniele A. Di Pietro <dipietro@unibg.it>
  \date 11-6-2005
*/

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP 1

#include "shape.hpp"

namespace Geometry 
{
  /*!
    \class Triangle
    \brief A class to handle triangular elements
    \author Daniele A. Di Pietro <dipietro@unibg.it>
  */
  class Triangle : public Shape 
  {
  public:
    /** @name Constructors 
     */
    //@{
    //! Constructor taking the ID of three points
    Triangle(Point P0, Point P1, Point P2) : Shape(3, 3) 
    {
      point(0) = P0;
      point(1) = P1;
      point(2) = P2;
    };
    //@}
    /** @name Methods
     */
    //@{
    //! Return the measure
    Real measure() const 
    { 
      return 0.5 * fabs( point(0)[0]*(point(1)[1] - point(2)[1]) - 
                         point(1)[0]*(point(0)[1] - point(2)[1]) +
                         point(2)[0]*(point(0)[1] - point(2)[1])); 
    };
    //! Return the \f$i^{\mathrm{th}}\f$ edges
    virtual Edge edge(const int& i);
    //@}
  };
}

#endif
