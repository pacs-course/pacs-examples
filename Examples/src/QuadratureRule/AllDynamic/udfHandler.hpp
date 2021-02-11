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
namespace apsc::NumericalIntegration{
  //! A class to simplify the management of loading funcions from a shared library
  /*!  Functions must have the signature determined by
    NumericalQuadrature::FunPoint or to be convertible to that
    signature.

    In particular the required signature is
    @code
    double ()(double const &)
    @endcode

    Functions in the library should be set with C linkage using the 
    extern 'C'{} block. Otherwise you should know their mangled name.
  */
  class UdfHandler{
  public:
    //! The type of function thet should be stored
    using fp_type=double (*)(double const &);
    static_assert(std::is_convertible<fp_type,apsc::NumericalIntegration::FunPoint>::value,
                  "The signature of the integrand function is not consistent");
    
    UdfHandler()=default;
    //! destructor closes library
    ~UdfHandler();
    //! Constructor that opens library
    //! @param s library name
    explicit UdfHandler(std::string const & s);
    //! Opens the library
    //! @param s the library name
    void openUdfLibrary(std::string const & s);
    //! Extract function from the library
    //! @param s function name
    //! @result The callable object storing the function
    apsc::NumericalIntegration::FunPoint getFunction(std::string const & s) const;
    //! Closes library
    void closeUdfLibrary();
  private:
    //! The library handle
    void * M_lib_handle=nullptr;
  };

}




#endif /* UDFHANDLER_HPP_ */
