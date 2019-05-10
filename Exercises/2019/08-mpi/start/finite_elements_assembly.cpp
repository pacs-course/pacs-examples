#include "sparse_matrix_simplified.h"

int
conn (int inode, int iel)
{
  int res = 0;
  int elcol, elrow;
  elcol = iel / 4;
  elrow = iel - elcol * 4;
  res = elcol * 5 + elrow;
  if (inode == 1) res += 1;
  if (inode == 2) res += 5;
  if (inode == 3) res += 6;
  return (res);
};

int
main (int argc, char *argv[])
{

  int is, ie, is_elems, ie_elems;
  sparse_matrix A;
  
  std::vector<int> row_ptr, col_ind;
  std::vector<double> value;
  
  A.resize (55);
  is_elems = 0; ie_elems = 40; is = 0; ie = 55;
  std::cout << "runing serially\n" ; is_elems = 0; ie_elems = 40; is = 0; ie = 55; 
   
  std::vector<std::vector<double>> locmatrix =
    {{2,-1,-1,0}, {-1,2,0,-1},{-1,0,2,-1},{0,-1,-1,2}};

  double tmp;
  for (int iel = is_elems; iel < ie_elems; ++iel)
    for (int inode = 0; inode < 4; ++inode)
      for (int jnode = 0; jnode < 4; ++jnode)
        {
          tmp = locmatrix[inode][jnode];
          if (tmp != 0.)
            A[conn (inode,iel)][conn (jnode,iel)] +=
              tmp;
        }

  A.csr (value, col_ind, row_ptr, 0);
  
 
  std::cout << A << std::endl;
  std::cout << "\n\n";
  
  std::cout << "a = [";
  for (auto ii : value)
    std::cout << ii << " ";
  std::cout << "];\n";
  
  std::cout << "col_ind = [";
  for (auto ii : col_ind)
    std::cout << ii << " ";
  std::cout << "];\n";
  
  std::cout << "row_ptr = [";
  for (auto ii : row_ptr)
    std::cout << ii << " ";          
  std::cout << "];\n";
            

  return 0;
}
