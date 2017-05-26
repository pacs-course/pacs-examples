#ifndef HAVE_SPARSEMATRIX_H
# define HAVE_SPARSEMATRIX_H

#include <iostream>
#include <map>
#include <vector>

class matrix : public std::vector<std::map<int, double>>
{
public:
  matrix (int N) { (*this).resize (N); };

  std::vector<double>
  operator* (std::vector<double> x)
  {
    std::vector<double> out (x.size (), .0);
    for (int icol = 0; icol < (*this).size (); ++icol)
      for (auto jrow = (*this)[icol].begin (); jrow != (*this)[icol].end (); ++jrow)
        out[(*jrow).first] += (*jrow).second * x[(*jrow).first];
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
