/*
 * ModelBasis.hpp
 *
 *  Created on: Jan 24, 2019
 *      Author: forma
 */

#ifndef SRC_REGRESSION_POLYNOMIALBASIS_HPP_
#define SRC_REGRESSION_POLYNOMIALBASIS_HPP_
#include "RegressionTraits.hpp"
#include <functional>
#include <vector>
namespace LinearAlgebra
{
//! A class that contains classic monomial basis functions
template <LinearAlgebraLibrary L = LinearAlgebra::EIGEN>
class PolynomialMonomialBasisFunction
{
public:
  //! Should be passed as template parameter, but here I simplify things
  using Trait = RegressionTraits<L>;
  //! The type for BasisFuncrions
  using BasisFunctions = typename Trait::BasisFunctions;
  //! The type for a Vector
  using Vector = typename Trait::Vector;
  //! Construct the basis function for a polynomial space
  PolynomialMonomialBasisFunction(std::size_t n = 0u);
  //! Sets the basis functions on an existing object
  void setFunctions(std::size_t n);
  //! Returns the ith basis function
  auto
  getFunction(std::size_t i)
  {
    return this->M_basisFunctions[i];
  }
  //! Evaluation at a point
  Vector eval(const double &x) const;
  //! Evaluation of the derivatives at a point
  Vector derivatives(const double &x) const;
  //! The number of basis functions, i.e the capacity of the model
  std::size_t
  size() const
  {
    return M_basisFunctions.size();
  }
  //! If I want to know the degree of a the polynomial I am using
  int
  degree() const
  {
    return this->size() - 1;
  }

private:
  //! Here I store the basis functions for my model
  BasisFunctions M_basisFunctions;
  //! Derivatives are needed only if I use a gradient scheme
  /*! For the version that uses QR are useless, but I compute them
   *  for completeness
   */
  BasisFunctions M_derivatives;
};
} // namespace LinearAlgebra
#endif /* SRC_REGRESSION_POLYNOMIALBASIS_HPP_ */
// Include implementation. The header file with the implementation has its own
// header guard, so I can include it after the include guard of this file.
#include "PolynomialBasis_impl.hpp"
