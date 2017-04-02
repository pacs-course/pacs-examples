#include <iostream>
#include <vector>
#include "legendre_rule.hpp"

int main()
{
  int nt = 4;

  std::vector<double> nds(nt);
  std::vector<double> wts(nt);

  cdgqf ( nt, 1, 1., 1., &nds[0], &wts[0] );

  for( int k = 0; k < nt; k++ )
  {
    std::cout << "nd = " << std::setprecision(16) << nds[k] << "\t"
              << "wt = " << std::setprecision(16) << wts[k] << std::endl;
  }

  return 0;
}

