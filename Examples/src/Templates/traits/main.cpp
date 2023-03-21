#include <array>
#include <cmath>
#include <complex>
#include <iostream>
#include "innerProduct.hpp"



int
main()
{
  using namespace std;
  using namespace apsc;
  std::array<double, 2> xr = {{3., 4.}};
  std::array<double, 2> yr = {{6., 7.}};
  // Note the extra parenthesis. Necessary for the array
  std::array<std::complex<double>, 2> xc = {{{3., 4.}, {-1.0, 4.5}}};
  std::array<std::complex<double>, 2> yc = {{{6., 7.}, {0.0, 1.0}}};

  cout << "innerproduct of arrays of double = " << innerProduct(xr, yr) << '\n';
  cout << "innerproduct of arrays of complex= " << innerProduct(xc, yc) << '\n'
       << "\n";
}
