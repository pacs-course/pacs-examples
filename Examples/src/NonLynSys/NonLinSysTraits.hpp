/*
 * NonLinSysTraits.hpp
 *
 *  Created on: Feb 12, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_NONLYNSYS_NONLINSYSTRAITS_HPP_
#define EXAMPLES_SRC_NONLYNSYS_NONLINSYSTRAITS_HPP_
#include "Eigen/Core"
#include <functional>
#include <vector>
namespace apsc
{
namespace NonLinSysTraits
{
  /*!
   * @brief The trait for a system \f$ S^n \to S^m\f$ where S is a scalar field
   * @details The system is represented by a vector of functions
   * 
   * @tparam Scalar The scalar field type
   */
  template <class Scalar=double> struct VectorTraits
  {
    //! The type of the function argument
    using ArgumentType = std::vector<Scalar>;
    //! The type of the system result
    using ResultType = std::vector<Scalar>;
    //! The type of the result of a single equation
    using ScalarType = Scalar;
    //! The type of the single function
    using ScalarFunctionType = std::function<ScalarType (ArgumentType const &)>;
    //! The type that can be used to wrap the whole system
    using VectorFunctionType = std::function<ResultType (ArgumentType const &)>;
    //!  The container where the single functions are stored
    using FunctionContainerType = std::vector<ScalarFunctionType>;
  };

  template <class Scalar=double> struct EigenVectorTraits
  {
    using ArgumentType = Eigen::Matrix<Scalar, Eigen::Dynamic, 1>;
    using ResultType = ArgumentType;
    using ScalarType = Scalar;
    using ScalarFunctionType = std::function<ScalarType(ArgumentType const &)>;
    using VectorFunctionType = std::function<ResultType(ArgumentType const &)>;
    using FunctionContainerType = std::vector<ScalarFunctionType>;
  };

  /*!
   * Here  we show how to change traits to have a scalar as argument and not a
   * vector
   *
   * The system will be \f$ S \to S^m\f$ where S is a scalar field.
   *
   * @tparam Scalar
   */
  template <class Scalar=double> struct ScalarTraits
  {
    using ArgumentType = Scalar;
    using ResultType = std::vector<Scalar>;
    using ScalarType = Scalar;
    using ScalarFunctionType = std::function<ScalarType(ArgumentType const &)>;
    using VectorFunctionType = std::function<ResultType(ArgumentType const &)>;
    using FunctionContainerType = std::vector<ScalarFunctionType>;
  };

} // namespace NonLinSysTraits
} // namespace apsc

#endif /* EXAMPLES_SRC_NONLYNSYS_NONLINSYSTRAITS_HPP_ */
