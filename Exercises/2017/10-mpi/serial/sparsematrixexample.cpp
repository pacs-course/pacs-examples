#include "sparsematrix.h"

int
main ()
{
  constexpr int N = 10;
  matrix m (N);
  std::vector<double> x (N);
  
  for (int jcol = 0; jcol < N; ++jcol)
    {
      int startrow = jcol > 0 ? jcol - 1 : 0;
      int endrow = jcol < N-1 ? jcol + 1 : N-1;
      for (int irow = startrow;           
           irow <= endrow; ++irow)
        {
          if (irow == jcol)
            m[jcol][irow] = 2.0;
          else
            m[jcol][irow] = -1.0;
        }
    }

  for (int ii = 0; ii < N; ++ii)
    x[ii] = static_cast<double> (ii);

  std::cout << "m = " << std::endl;
  m.print ();

  std::cout << "x = " << std::endl;
  for (int ii = 0; ii < N; ++ii)
    std::cout << x[ii] << std::endl;

  std::vector<double> y = m * x;
  std::cout << "y = " << std::endl;
  for (int ii = 0; ii < N; ++ii)
    std::cout << y[ii] << std::endl;
  
}
