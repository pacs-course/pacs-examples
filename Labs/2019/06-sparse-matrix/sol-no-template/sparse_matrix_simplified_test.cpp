#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
//#include <sparse_matrix_simplified.h>

constexpr int n = 10;

using sparse_matrix = std::vector<std::map<int, double>>;

std::ostream &
operator<< (std::ostream &stream,
            sparse_matrix &sp)
{
  stream << "mat = spconvert ([" ;
  for (size_t i = 0; i < sp.size (); ++i)
    {
      if (sp[i].size ())
        for (auto j = sp[i].begin (); j != sp[i].end (); ++j)
          {
            stream << i+1 << ", " << (*j).first + 1 << ", ";
            stream  << std::setprecision (17)
                    << (*j).second << ";" << std::endl;
          }
    }
  stream << "]);" << std::endl ;
  return stream;
}


int main () {

  std::vector<double> x (n, 1.0);
  sparse_matrix A;
  A.resize (n);

  for (int ii = 0; ii < n; ++ii)
    {
      if (ii > 0)
        A[ii][ii-1] = -1;
      
      if (ii < n-1)
        A[ii][ii+1] = -1;
      
      A[ii][ii] = 4;
    }


  std::cout << "Loop through matrix entries :" << std::endl;
  for (int ii = 0; ii < n; ++ii)
    for (auto const& [key, val] : A[ii])
      std::cout << "A[" << ii << "][" << key << "] = " << val << std::endl;
  std::cout << std::endl << std::endl;

  
  std::cout << "Stream operator :" << std::endl;
  std::cout << A;
  std::cout << std::endl;

  
  std::vector<int> irow, jcol;
  std::vector<double> v;

  /*
  A.csr (v, jcol, irow);

  std::cout << "CSR vectors :" << std::endl;
  for (auto ii : irow)
    std::cout << ii << " ";
  std::cout << std::endl;

  for (auto ii : jcol)
    std::cout << ii << " ";
  std::cout << std::endl;

  for (auto ii : v)
    std::cout << ii << " ";
  std::cout << std::endl << std::endl;

  std::cout << "Matrix entries from CSR :" << std::endl;
  for (int ii = 0; ii < n; ++ii)
    for (int jj = irow[ii]; jj < irow[ii+1]; ++jj)
      std::cout << "A[" << ii << "][" << jcol[jj]<< "] = " << v[jj] << std::endl;
  */
}
