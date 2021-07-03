/*
 * NonLinSysTraits.hpp
 *
 *  Created on: Feb 12, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_NONLYNSYS_NONLINSYSTRAITS_HPP_
#define EXAMPLES_SRC_NONLYNSYS_NONLINSYSTRAITS_HPP_
#include <vector>
#include <functional>
#include "Eigen/Core"
namespace apsc
{
  namespace NonLinSysTraits
  {
    /*!
     * The trait for a system \f$ S^n \to S^m\f$ where S is a scalar field
     *
     * @tparam Scalar The scalar field type
     */
    template<class Scalar>
    struct VectorTraits
    {
      using ArgumentType=std::vector<Scalar>;
      using ResultType=std::vector<Scalar>;
      using ScalarType=Scalar;
      using ScalarFunctionType = std::function<ScalarType (ArgumentType const &)>;
      using VectorFunctionType = std::function<ResultType (ArgumentType const &)>;
      using SystemType = std::vector<ScalarFunctionType>;
    };

    template<class Scalar>
    struct EigenVectorTraits
    {
      using ArgumentType=Eigen::Matrix<Scalar,Eigen::Dynamic,1>;
      using ResultType=ArgumentType;
      using ScalarType=Scalar;
      using ScalarFunctionType = std::function<ScalarType (ArgumentType const &)>;
      using VectorFunctionType = std::function<ResultType (ArgumentType const &)>;
      using SystemType = std::vector<ScalarFunctionType>;
    };


    /*!
     * Here  we show how to change traits to have a scalar as argument and not a vector
     *
     * The system will be \f$ S \to S^m\f$ where S is a scalar field.
     *
     * @tparam Scalar
     */
    template<class Scalar>
    struct ScalarTraits
    {
      using ArgumentType=Scalar;
      using ResultType=std::vector<Scalar>;
      using ScalarType=Scalar;
      using ScalarFunctionType = std::function<ScalarType (ArgumentType const &)>;
      using VectorFunctionType = std::function<ResultType (ArgumentType const &)>;
      using SystemType = std::vector<ScalarFunctionType>;
    };

  }
}



#endif /* EXAMPLES_SRC_NONLYNSYS_NONLINSYSTRAITS_HPP_ */
