#ifndef HH_QUADRATURERULEBASE_HH
#define HH_QUADRATURERULEBASE_HH
#include "QuadratureRuleTraits.hpp"
#include <functional>
#include <memory>
#include <string>

namespace apsc::NumericalIntegration
{
//! The basis class for all the basic integration rules
/*
  This basis class is the common class for all numerical integration
  formulae that approximate the integral \f$ \int_{-1}^{1} f(y) dy\f$.
  It's a very light class (no variable members) that provides
  the common interface to all integration rules.

  Constructors and assignment operators
  are not defined since the synthetic ones are sufficient.

  @note This class is an interface (no data members). It is done on purpose.
*/
class QuadratureRuleBase
{
public:
  //! The class is clonable.
  /*!
    Having a clonable class makes it possible to write copy constructors
    and assignment operators for classes that aggregate object of the
    QuadratureRule hierarchy by composition.
    @return a unique pointer to this base class
  */
  virtual std::unique_ptr<QuadratureRuleBase> clone() const = 0;
  // Applies the rule in the interval (a,b)

  virtual double apply(FunPoint const &f, double const &a,
                       double const &b) const = 0;
  virtual ~QuadratureRuleBase() = default;
  /* To be able to use the rule in the context of adaptive quadrature
    when I will load rules dynamically I need to enrich the interface
    with these functions that in case of normal rules do just nothing.
    @note We cannot make them abstract since I am not redefining them
    in non-adaptive rules!
    */
  /*!
   *  Sets the target error of an adaptive rule
   */
  virtual void
  setTargetError(double const)
  {}
  /*!
   * Sets the maximal level of refinement in an adaptive rule
   */
  virtual void
  setMaxIter(unsigned int)
  {}
  //! a string that identify the general type of quadrature rule
  virtual std::string name() const = 0;
};
//! The type of the object holding the quadrature rule.
// using QuadratureRuleHandler=std::unique_ptr<QuadratureRule>;
//! I am using the PoiterWrapper for clonable classes defined in
//! CloningUtilities.hpp
using QuadratureRuleHandler = apsc::PointerWrapper<QuadratureRuleBase>;
} // namespace apsc::NumericalIntegration
#endif
