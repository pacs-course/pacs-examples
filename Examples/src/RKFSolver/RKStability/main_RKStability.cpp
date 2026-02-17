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
#include <numbers>
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
  auto [x, status] =
    compute.computeLimit(3. * std::numbers::pi_v<double> / 4.);
  std::cout << x << " status=" << status << std::endl;
  compute.options.verbose = true;
  auto          values = compute.computeStabilityRegion(1000, {-1., 1.0});
  std::ofstream out("stability.dat");
  for(auto const &xp : values)
    out << xp.real() << " " << xp.imag() << "\n";
}
