#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>

int main ()
{

  constexpr double a = 0;
  constexpr double b = 1;
  constexpr double L = b - a;
  constexpr unsigned int nnodes = 50;
  constexpr unsigned int nels = nnodes - 1;
  constexpr double h = L / static_cast<double> (nels);
  constexpr unsigned int maxit = 10000;
  constexpr double tol = 1.0e-15;
  
  std::array<double, nnodes> nodes;
  for (unsigned int ii = 0; ii < nnodes; ++ii)
    nodes[ii] = static_cast<double>(ii) * h;

  std::array<std::array<unsigned int, 2>, nels> elements;
  for (unsigned int ii = 0; ii < nels; ++ii)
    {
      elements[ii][0] = ii;
      elements[ii][1] = ii+1;
    }

  std::array<std::array<double, nnodes>, nnodes> A;
  for (unsigned int ii = 0; ii < nnodes; ++ii)
    A[ii].fill (0.0);
  
  for (unsigned int iel = 0; iel < nels; ++iel)
    {
      std::array<std::array<double, 2>, 2> mloc;
      for (unsigned int ii = 0; ii < 2; ++ii)
        mloc[ii].fill (0.0);
      
      for (unsigned int inode = 0; inode < 2; ++inode)
        {
          double igrad = (inode == 0 ? 1.0 / h : -1.0 / h);
          for (unsigned int jnode = 0; jnode < 2; ++jnode)
            {
              double jgrad =  (jnode == 0 ? 1.0 / h : -1.0 / h);
              mloc[inode][jnode] = igrad * jgrad * h;
              A[elements[iel][inode]][elements[iel][jnode]] += 
                mloc[inode][jnode];
            }
        }
    }

  std::array<double, nnodes> f;
  f.fill (0.0);
  for (unsigned int iel = 0; iel < nels; ++iel)
    {
      std::array<double, 2> vloc;
      vloc.fill (0.0);
      
      for (unsigned int inode = 0; inode < 2; ++inode)
        {
          vloc[inode] = h / 2.0;
          f[elements[iel][inode]] += vloc[inode];            
        }
    }

  f[0] = 0;
  f.back () = 0;

  A[0][0] = 1.0;
  std::fill (&(A[0][1]), A[0].end (), 0.0);

  A.back ().back () = 1.0;
  std::fill (A.back ().begin (), A.back ().end () - 1, 0.0);

  std::array<double, nnodes> uh (f);
  double uh_new = 0;
  double incrnorm = 0;
  for (unsigned int ii = 0; ii < maxit; ++ii)
    {
      incrnorm = 0;
      for (unsigned int jj = 0; jj < nnodes; ++jj)
        {
          double res = f[jj];

          for (unsigned int kk = 0; kk < nnodes; ++kk)
            if (kk != jj)
              res -= A[jj][kk] * uh[kk];
          
          uh_new = res / A[jj][jj];
          incrnorm = std::abs (uh_new - uh[jj]) > incrnorm ?
            std::abs (uh_new - uh[jj]) :
            incrnorm;
          uh[jj] = uh_new;
        }

      std::cerr << ii << " " << "incrnoorm = "<< incrnorm << std::endl; 
      if (incrnorm < tol)
        {
          std::cerr << "converged!!" << std::endl; 
          break;
        }
    }


  for (unsigned int ii = 0; ii < nnodes; ++ii)
    std::cout << uh[ii] << std:: endl;

  return 0;
};



