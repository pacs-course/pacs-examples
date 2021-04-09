/*
 * udfHandler.hpp
 *
 *  Created on: 28/nov/2010
 *      Author: formaggia
 */

#ifndef UDFHANDLER_HPP_
#define UDFHANDLER_HPP_
#include <string>
#include <stdexcept>
#include <type_traits>
#include "QuadratureRuleBase.hpp"
#include "Factory.hpp"

namespace apsc::NumericalIntegration{

  using IntegrandFactory= GenericFactory::FunctionFactory
    <std::string,
    apsc::NumericalIntegration::FunPoint
    >;

  extern IntegrandFactory& myIntegrands;

  template <class FunctionObject>
  void addIntegrandToFactory(std::string const & name, FunctionObject&& integrand)
  {
    static_assert(std::is_convertible_v<FunctionObject,apsc::NumericalIntegration::FunPoint>,"Function object type not good for integrand\n");
    myIntegrands.add(name,std::forward<FunctionObject>(integrand));
  }

}




#endif /* UDFHANDLER_HPP_ */
