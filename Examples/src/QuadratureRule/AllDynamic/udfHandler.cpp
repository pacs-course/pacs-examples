#include "udfHandler.hpp"
#include <dlfcn.h>

namespace apsc::NumericalIntegration
{
IntegrandFactory &myIntegrands = IntegrandFactory::Instance();
}
