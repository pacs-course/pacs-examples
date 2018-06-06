#include <octave/oct.h>

DEFUN_DLD(dld,args,nargout,"dld (array) \nreturn the elements of the array in reverse order\n")
{
  octave_value_list retval;
  int nargin = args.length ();

  if (nargin != 1)
    print_usage ();
  else
    {
      Array<double> a = args(0).array_value ();
      if (! error_state)
        {
          Array<double> b (a);
          double* ap = a.fortran_vec ();
          double* bp = b.fortran_vec ();
          for (octave_idx_type i = a.numel () - 1, j = 0; i >= 0; i--, j++)
            bp[i] = ap[j];
          retval = octave_value (b);
        }
    }
  return retval;
}
