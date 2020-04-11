/*
 * RungeKuttaTraits.hpp
 *
 *  Created on: Feb 26, 2020
 *      Author: forma
 */

#ifndef RKSOLVER_RKFTRAITS_HPP_
#define RKSOLVER_RKFTRAITS_HPP_
#include <vector>
#include <functional>
#include <cmath>
#include "Eigen/Dense"
namespace apsc
{
  //! The two kinds of RK solver for scalar or vector differential probelms
  enum class RKFKind {SCALAR=0,VECTOR=1,MATRIX=2};
  //! I hide this part since not of general use
  //!@{
  //! Primary template
  template <RKFKind Kind>
  struct RKFTraits{};

  //! Specialization for scalar
  template <>
  struct RKFTraits<RKFKind::SCALAR>
  {
    using VariableType=double;
    using ForcingTermType=std::function<VariableType (double const &, VariableType const &)>;
    static double norm(VariableType const & x) {return std::abs(x);}
  };

  //! Specialization for vector
  template <>
  struct RKFTraits<RKFKind::VECTOR>
  {
    using VariableType=Eigen::VectorXd;
    using ForcingTermType=std::function<VariableType (double const &, VariableType const &)>;
    static double norm(VariableType const & x){return x.norm();}
  };
  //! Specialization for marices
  template <>
  struct RKFTraits<RKFKind::MATRIX>
  {
    using VariableType=Eigen::MatrixXd;
    using ForcingTermType=std::function<VariableType (double const &, VariableType const &)>;
    static double norm(VariableType const & x){return x.norm();}
  };  //@}
}




#endif /* RKSOLVER_RKFTRAITS_HPP_ */
