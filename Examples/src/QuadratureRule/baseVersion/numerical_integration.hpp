#ifndef NUMERICAL_INTEGRATION_HPP
#define NUMERICAL_INTEGRATION_HPP
#include "mesh.hpp"

#include "QuadratureRuleBase.hpp"
namespace apsc::NumericalIntegration
{
using namespace Geometry;

/*!
  Class for composite integration.

  It is implemented using the following design:
  - Composition with a QuadraturRule is implemented via a
    unique_ptr<>. A Quadrature object thus owns a polymorphic object
    of type QuadratureRule. This allows to assign the quadrature rule
    run-time.

  - A 1D mesh is simply aggregated as an object. So a copy is
    made. We however support move semantic (introduced in C++11) so that
    if the mesh class implements a move constructor we can move the
    mesh into the Quadrature, saving memory.

    It is an example of the Bridge Design pattern: part of the implementation
    is delegated to a polymorphic object (the QuadratureRule).
 */
class Quadrature
{
public:
  typedef apsc::NumericalIntegration::FunPoint FunPoint;
  //! Constructor.
  /*!
    \param rule A unique_ptr storing the rule.
    \param mesh The 1D mesh (passed via universal reference)
   */
  template <typename QuadHandler, typename MESH>
  Quadrature(QuadHandler &&rule, MESH &&mesh)
    : rule_(std::forward<QuadHandler>(rule)), mesh_(std::forward<MESH>(mesh))
  {}

  //! A second constructor
  /*!  In this case we pass the object and we use the fact that the
    QuadratureRule classes are Clonable (that is they contain a
    clone() method). So, I can safely pass also a reference to a
    QuadratureRule base class.  If QuadratureRule where not clonable
    it would not work.

    \param rule The rule.
    \param mesh The 1D mesh
   */
  template <typename MESH>
  Quadrature(const QuadratureRuleBase &rule, MESH &&mesh)
    : rule_(rule.clone()), mesh_(std::forward<MESH>(mesh))
  {}
  //! Copy constructor.
  /*!
    \todo I could have used the Wrapper class in cloningUtilities.hpp and save
    the need of building copy/move and assignement operator. I could have
    used the synthetic ones. Since instead I am storing a unique_ptr,
    if I want to make the class copiable/movable with a deep copy, I need
    to write the operators myself, exploiting clone().
   */
  Quadrature(Quadrature const &rhs) = default;
  /*!
   * Move constructor.
   * @param rhs The input quadrature
   */
  Quadrature(Quadrature &&rhs) = default;
  //! Copy assignment.
  Quadrature &operator=(Quadrature const &) = default;
  //! Move assignment.
  Quadrature &operator=(Quadrature &&) = default;
  //! Calculates the integal on the passed integrand function.
  double apply(FunPoint const &) const;
  QuadratureRuleBase const &
  myRule() const
  {
    return *(rule_.get());
  }

protected:
  QuadratureRuleHandler rule_;
  Mesh1D                mesh_;
};

/*
  (-1,1) ->(a,b) y*(b-a)/2 + (a+b)/2
  wfactor dx/dy = h/2
 */

} // namespace apsc::NumericalIntegration

#endif
