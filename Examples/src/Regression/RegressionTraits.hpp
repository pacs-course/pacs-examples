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
  //! Structure containing basic type definition
  struct RegressionTraitsDefault
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
}




#endif /* SRC_REGRESSION_REGRESSIONTRAITS_HPP_ */
