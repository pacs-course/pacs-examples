#include "sparsematrix.h"

int
main ()
{
  constexpr int N = 5;
  matrix m (N);
  for (int jcol = 0; jcol < N; ++jcol)
    {
      // int startrow = jcol > 0 ? jcol - 1 : 0;
      // int endrow = jcol < N-1 ? jcol + 1 : N-1;
      // std::cout << "jcol = " << jcol << std::endl;
      for (int irow = startrow;           
           irow <= endrow; ++irow)
        {
          std::cout << "  irow = " << irow << std::endl;
          if (irow == jcol)
            m[jcol][irow] = 2.0;
          else
            m[jcol][irow] = -1.0;
        }
    }

  m.print ();
  
}
