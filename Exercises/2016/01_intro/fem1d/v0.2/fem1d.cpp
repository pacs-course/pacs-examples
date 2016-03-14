#include "fem1d.h"

int main ()
{

  mesh m (a, b, nnodes);
  
  double A[nnodes][nnodes];
  std::fill (&(A[0][0]), &(A[nnodes-1][nnodes]), 0.0);
    
  for (unsigned int iel = 0; iel < m.nels; ++iel)
    {

      double mloc[2][2];
      std::fill (&(mloc[0][0]), &(mloc[1][2]), 0.0);
      
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

  double f[nnodes];
  std::fill (f, f + nnodes, 0.0);
  
  for (unsigned int iel = 0; iel < m.nels; ++iel)
    {
      double vloc[2];
      std::fill (vloc, vloc + 2, 0.0);
      
      for (unsigned int inode = 0; inode < 2; ++inode)
        {
          vloc[inode] = m.h / 2.0;
          f[m.elements[iel][inode]] += vloc[inode];            
        }
    }

  f[0] = 0;
  f[nnodes - 1] = 0;

  A[0][0] = 1.0;
  std::fill (&(A[0][1]), &(A[0][nnodes]), 0.0);

  A[nnodes-1][nnodes-1] = 1.0;
  std::fill (&(A[nnodes-1][0]), &(A[nnodes-1][nnodes-1]), 0.0);

  double uh[nnodes];
  gauss_seidel (A, f, uh);

  for (unsigned int ii = 0; ii < nnodes; ++ii)
    std::cout << uh[ii] << std:: endl;
      
  return 0;
};



