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
#include "QuadratureRule.hpp"
namespace NumericalIntegration{
  //! A class to simplify the management of loading funcions from a shared library
  /*!
    Functions must have the signature determined by NumericalQuadrature::FunPoint or
    to be convertible to that signature.

    In particular the required signature is
    @code
    double ()(double const &)
    @endcode
  */
  class UdfHandler{
  public:
    using fp_type=double (*)(double const &);
    static_assert(std::is_convertible<fp_type,NumericalIntegration::FunPoint>::value,
                  "The signature of the integrand function is not consistent");
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
