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
#include "QuadratureRule.hpp"
namespace NumericalIntegration{
  class UdfHandler{
  public:
    
  UdfHandler();
  ~UdfHandler();
  explicit UdfHandler(std::string const & s);
  void openUdfLibrary(std::string const & s);
  NumericalIntegration::FunPoint getFunction(std::string const & s) const;
  void closeUdfLibrary();
private:
  void * M_lib_handle;
};

}




#endif /* UDFHANDLER_HPP_ */
