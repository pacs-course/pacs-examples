#include "sparse.h"

unsigned int
sparse::get_rows () const
{
  unsigned int retval = this->length ();
  return retval;
};
