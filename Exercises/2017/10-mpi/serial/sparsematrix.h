#ifndef HAVE_SPARSEMATRIX_H
# define HAVE_SPARSEMATRIX_H

#include <iostream>
#include <map>
#include <vector>

class matrix : public std::vector<std::map<int, double>>
{
private:

  const int nrows;
  const int ncols;

public:

  matrix (int N) : nrows (N), ncols (N) { (*this).resize (N); };
  matrix (int NR, int NC) : nrows (NR), ncols (NC) { (*this).resize (NC); };
  
  std::vector<double>
  operator* (std::vector<double> x)
  {
    std::vector<double> out (nrows, .0);
    for (int icol = 0; icol < (*this).size (); ++icol)
      for (auto jrow = (*this)[icol].begin (); jrow != (*this)[icol].end (); ++jrow)
        out[(*jrow).first] += (*jrow).second * x[icol];
    return out;
  };

  void
  print ()
  {
    for (int icol = 0; icol < (*this).size (); ++icol)
      {
        std::cout << "column " << icol << " ";
      for (auto jrow = (*this)[icol].begin (); jrow != (*this)[icol].end (); ++jrow)
        {
          std::cout << "(" << (*jrow).first << ","
                    <<(*jrow).second
                    << ")  ";
        }
      std::cout << std::endl;
      }
  };

};

#endif
