#ifndef __INT2D_HPP
#define __INT2D_HPP 1

#include <vector>
#include "mesh2D.hpp"

/*!
  \file int2D.hpp
  \brief delegating the rule over a mesh

   Here the quadrature integration on the triangle \f$T\f$ is handled
   by a specific private method of the classe, while the method apply
   takes care of implementing the integration in all the domain.
*/
namespace NumericalIntegration{

  typedef double (*FunPoint)(double const &, double const &);

  class Quadrature
  {

  public:

    /*!
      \brief The constructor

      \param mesh The mesh where for the integration.

     */
    Quadrature( const Geometry::Mesh2D & mesh );

    //! It applies the method applyTriangle
    double apply( FunPoint f , int rule );

  protected:

    void triangleFekete( const Geometry::Triangle& T, double* Tf);

    double applyTriangle( FunPoint f, const Geometry::Triangle&, int rule );

    const Geometry::Mesh2D & M_mesh;

  };
}
#endif
