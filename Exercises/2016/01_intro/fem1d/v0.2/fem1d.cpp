#include <iostream>
#include "fem1d.h"
#include "config.h"

int main ()
{

  mesh m (a, b, nnodes);
  
  double A;
  for (unsigned int ii = 0; ii < nnodes; ++ii)
    A[ii].fill (0.0);
  
  for (unsigned int iel = 0; iel < m.nels; ++iel)
    {
      std::array<std::array<double, 2>, 2> mloc;
      for (unsigned int ii = 0; ii < 2; ++ii)
        mloc[ii].fill (0.0);
      
      for (unsigned int inode = 0; inode < 2; ++inode)
        {
          double igrad = (inode == 0 ? 1.0 / m.h : -1.0 / m.h);
          for (unsigned int jnode = 0; jnode < 2; ++jnode)
            {
              double jgrad =  (jnode == 0 ? 1.0 / m.h : -1.0 / m.h);
              mloc[inode][jnode] = igrad * jgrad * m.h;
              A[m.elements[iel][inode]][m.elements[iel][jnode]] += 
                mloc[inode][jnode];
            }
        }
    }

  std::array<double, nnodes> f;
  f.fill (0.0);
  for (unsigned int iel = 0; iel < m.nels; ++iel)
    {
      std::array<double, 2> vloc;
      vloc.fill (0.0);
      
      for (unsigned int inode = 0; inode < 2; ++inode)
        {
          vloc[inode] = m.h / 2.0;
          f[m.elements[iel][inode]] += vloc[inode];            
        }
    }

  f[0] = 0;
  f.back () = 0;

  A[0][0] = 1.0;
  std::fill (&(A[0][1]), A[0].end (), 0.0);

  A.back ().back () = 1.0;
  std::fill (A.back ().begin (), A.back ().end () - 1, 0.0);

  std::array<double, nnodes> uh (f);
  gauss_seidel<nnodes> (A, f, uh);

  for (unsigned int ii = 0; ii < nnodes; ++ii)
    std::cout << uh[ii] << std:: endl;
      
  return 0;
};



