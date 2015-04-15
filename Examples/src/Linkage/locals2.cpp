#include "globals.hpp"
#include "locals2.hpp"
#ifdef  NOGLOBAL
static int globalValue;
#endif
//! If you wish you can use :: to specify the global scope
void zeroGlobal(){ ::globalValue=0;}
