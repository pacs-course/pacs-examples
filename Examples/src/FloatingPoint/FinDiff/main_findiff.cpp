#include "GetPot"
#include <cmath>
#include <complex>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>
#if defined(SINGLE_PRECISION)
using Real = float;
std::string filename("single.csv");
#elif defined(EXTENDED_PRECISION)
using Real = long double;
std::string filename("extended.csv");
#else
using Real = double;
std::string filename("double.csv");
#endif

// Global values (are outside any scope so I can change them in
// funct and dfunct in just one go)
constexpr Real a = 100.0;
constexpr Real b = 4.0;

// I use lambdas and I exploit the fact that constexpr are capured
// automatically!
//! \f$ f(x)=a e^{bx}\f$
auto funct = [](auto const &x) { return a * std::exp(b * x); };
//! \f$ f^\prime(x)= ab e^{bx}\f$
auto dfunct = [](auto const &x) { return a * b * std::exp(b * x); };

//! Tests of accuracy of finite differences
/*!
  It computes \f$ \frac{df}{dx}(x)\f$ at a given point x using second order
  centered finite differences. It outputs the error. The example wants to show
  that floating point round-off error finally dominates.
 */
int
main()
{
  // Get roundoff unit (a use of auto)
  const auto u = 0.5 * std::numeric_limits<Real>::epsilon();
  std::cout << "Roundoff unit=" << u << std::endl;
  // Read some data
  GetPot ifl("data.pot");
  // Initial spacing
  Real h = ifl("h", 0.1);
  std::cout << " Initial Spacing " << h << std::endl;
  // Number of subdivisions
  const auto n = ifl("n", 10u);
  std::cout << " Number of subdivisions " << n << std::endl;
  // Point where derivative is computed
  const Real x = ifl("x", 3.0);
  std::cout << " X value " << x << std::endl;

  std::vector<Real> derNumer;
  std::vector<Real> der4Numer;
  std::vector<Real> dercxNumer;
  std::vector<Real> Error;
  std::vector<Real> Error4;
  std::vector<Real> Errorcx;
  std::vector<Real> spacing;
  std::vector<Real> roundoffErrorEstimate;
  derNumer.reserve(n);
  der4Numer.reserve(n);
  dercxNumer.reserve(n);
  Error.reserve(n);
  Error4.reserve(n);
  Errorcx.reserve(n);
  spacing.reserve(n);
  roundoffErrorEstimate.reserve(n);

  Real constexpr half = 0.5;
  Real constexpr twelvth = 1. / 12.;
  Real constexpr twothird = 2. / 3;

  for(unsigned int i = 0; i < n; ++i)
    {
      spacing.emplace_back(h);
      // Second order formula
      Real dn = half * (funct(x + h) - funct(x - h)) / h;
      // Fourth order formula
      Real dn4 = (twelvth * funct(x - 2 * h) - twothird * funct(x - h) -
                  twelvth * funct(x + 2 * h) + twothird * funct(x + h)) /
                 h;
      // with(out) a difference!
      Real dncx = std::imag(funct(std::complex<Real>{x, h})) / h;
      // Exact value
      Real de = dfunct(x);
      // Save results
      derNumer.emplace_back(dn);
      der4Numer.emplace_back(dn4);
      dercxNumer.emplace_back(dncx);
      Error.emplace_back(std::abs(de - dn));
      Error4.emplace_back(std::abs(de - dn4));
      Errorcx.emplace_back(std::abs(de - dncx));
      roundoffErrorEstimate.emplace_back(std::abs(u * dfunct(x) * x / h));
      h /= 2.;
    }
  // Write data
  std::ofstream file(filename.c_str());
  auto constexpr max_prec = std::numeric_limits<Real>::max_digits10;
  file << "# h,\t diff2,\t error2,\t truncation error,\t error4,\t error "
          "without_a diff\n";
  file.precision(
    max_prec); // do this if you want the data printed in full precision!
  // file.precision(8); // do this if you want the data printed in a smaller
  // precision!
  for(unsigned int i = 0; i < n; ++i)
    {
      file << std::scientific << spacing[i] << ",\t" << derNumer[i] << ",\t"
           << Error[i] << ",\t" << roundoffErrorEstimate[i] << ",\t"
           << Error4[i] << ",\t" << Errorcx[i] << std::endl;
    }
  file.close();
}
