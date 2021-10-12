#include "thomas.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
double
diff(std::vector<double> const &x, std::vector<double> const &y)
{
  double res{0.0};
  for(std::size_t i = 0; i < x.size(); ++i)
    res += (x[i] - y[i]) * (x[i] - y[i]);
  return std::sqrt(res) / x.size();
}

int
main()
{
  using namespace apsc;
  // the classic Laplacian (remember that b[0] and c[n-1] are not used!
  constexpr std::size_t n = 100;
  std::vector<double>   a, b, c, vec;
  for(std::size_t i = 0; i < n; ++i)
    {
      a.push_back(2.0);
      b.push_back(-1.0);
      c.push_back(-1.0);
      //      vec.push_back( 10.+(i%15));
      vec.push_back(1.0);
    }
  auto rhs = matVecTrid(a, b, c, vec);
  auto sol = thomasSolve(a, b, c, rhs);
  auto error = diff(vec, sol);
  //  std::cout<<std::scientific();
  std::cout << "Error =" << error << std::endl;
  // Now with symmetry
  rhs = matVecTrid(a, b, c, vec, true);
  sol = thomasSolveSym(a, b, c, rhs);
  error = diff(vec, sol);
  std::cout << "Error =" << error << std::endl;
}
