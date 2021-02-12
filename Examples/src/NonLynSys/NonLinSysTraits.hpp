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
namespace apsc
{
  namespace NonLinSysTraits
  {
    template<class Scalar>
    struct Vector
    {
      using ArgumentType=std::vector<Scalar>;
      using ResultType=std::vector<Scalar>;
      using ScalarType=Scalar;
      using ScalarFunctionType = std::function<ScalarType (ArgumentType const &)>;
      using VectorFunctionType = std::function<ResultType (ArgumentType const &)>;
      using SystemType = std::vector<ScalarFunctionType>;
    };
  }
}



#endif /* EXAMPLES_SRC_NONLYNSYS_NONLINSYSTRAITS_HPP_ */
