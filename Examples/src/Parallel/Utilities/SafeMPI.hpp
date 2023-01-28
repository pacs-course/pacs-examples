/* It allows to activate full warnings during ocmpilation and include mpi.h without 
   having the terminal swamped by rubbish.
*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wcast-function-type"
#include <mpi.h>
#pragma GCC diagnostic pop
