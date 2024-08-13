/*
 * main_RKStability.cpp
 *
 *  Created on: Nov 22, 2020
 *      Author: forma
 */
#include "ButcherRKF.hpp"
#include "computeRforRK.hpp"
#include <array>
#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
constexpr double pi = 3.141592653589793238462643383279;
int
main()
{
  using namespace apsc;
  using namespace apsc::RKFScheme;

  apsc::computeRforRK<apsc::RKFScheme::RK45_t, 2> compute;
  computeRforRK<RK45_t, 2>::complex               z = {0, 0};
  auto                                            res = compute(z);
  std::cout << res << std::endl;
  std::cout << compute({-1., 0.}) << std::endl;
  std::cout << compute({0., 1.}) << std::endl;
  auto [x, status] = compute.computeLimit(3. * pi / 4.);
  std::cout << x << " status=" << status << std::endl;
  compute.options.verbose = true;
  auto          values = compute.computeStabilityRegion(1000, {-1., 1.0});
  std::ofstream out("stability.dat");
  for(auto xp = values.begin(); xp != values.end(); ++xp)
    out << xp->real() << " " << xp->imag() << "\n";
}
