/*
 * RegressionTraits.hpp
 *
 *  Created on: Jan 24, 2019
 *      Author: forma
 */

#ifndef SRC_REGRESSION_REGRESSIONTRAITS_HPP_
#define SRC_REGRESSION_REGRESSIONTRAITS_HPP_
#include <functional>
#include <vector>
#include "Eigen/Core"
#include "Eigen/Dense"

namespace LinearAlgebra
{
  //! An enum that allow to distinguish different traits
  /*!
   *  At the moment only EIGEN is implemented
   */
  enum LinearAlgebraLibrary {EIGEN, ARMADILLO};
  // ! The primary template.
  template <LinearAlgebraLibrary>
  struct
  RegressionTraits{}; //!< Empty. I use only specializations!

  //!The specialization for Eigen classes
  template<>
  struct RegressionTraits<EIGEN>
  {
    using Function = std::function<double (double const &)>;// @suppress("Symbol is not resolved") @suppress("Type cannot be resolved")
    using BasisFunctions = std::vector<Function>; // @suppress("Invalid template argument")
    using Vector=Eigen::VectorXd;
    using Matrix=Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>;
    // I store the parameters in a Vector
    using Parameters=Vector;
    // Also the const function is internally wrapped into a std::function
    using CostFunction=Function;
  };
  //! The default trait
  using RegressionTraitsDefault=RegressionTraits<EIGEN>;
}




#endif /* SRC_REGRESSION_REGRESSIONTRAITS_HPP_ */
