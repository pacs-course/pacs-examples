#include <dlfcn.h>
#include "udfHandler.hpp"

namespace NumericalIntegration{

  UdfHandler::UdfHandler():M_lib_handle(nullptr){}
  
  void UdfHandler::openUdfLibrary(const std::string & s)
  {
    if (this->M_lib_handle)dlclose(this->M_lib_handle);
    this->M_lib_handle=dlopen(s.c_str(),RTLD_LAZY);
    if (!this->M_lib_handle)
      throw std::invalid_argument("Udf library " + s + " not found");
  }
  
  FunPoint UdfHandler::getFunction(const std::string & s) const
  {
    if (!this->M_lib_handle)
      throw std::invalid_argument("Trying to access function on unopened library");
    void * funp=dlsym(this->M_lib_handle,s.c_str());
    if (!funp)
      throw std::invalid_argument("Integrand named " +s+ " not found in library");
    else
      return FunPoint(reinterpret_cast<double (*)(double const &)>(funp));
  }
  
void UdfHandler::closeUdfLibrary(){
	if(!this->M_lib_handle) dlclose(this->M_lib_handle);
}

  UdfHandler::UdfHandler(const std::string & s):UdfHandler()
{
	this->openUdfLibrary(s);
}

UdfHandler::~UdfHandler(){this->closeUdfLibrary();}

}
