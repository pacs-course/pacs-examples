/*
 * mainHornerConstexpr.cpp
 *
 *  Created on: May 22, 2020
 *      Author: forma
 */
#include "HornerConstexpr.hpp"
#include <array>
#include <complex>
#include <iostream>
#include <vector>
int
main()
{
  // I open a block scope because I am lazy
  {
    std::vector<double>             coeffv{2., 4., 7.};
    constexpr std::array<double, 3> coeffa{2., 4., 7.};
    double                          x1 = 12.3;
    constexpr double                x2 = 12.3;
    // this will be inlined
    auto r1 = HornerConstexpr(coeffv, x1);
    // this is computed statically by the compiler
    auto r2 = HornerConstexpr(coeffa, x2);
    std::cout << "r1= " << r1 << " r2=" << r2 << std::endl;
  }
  {
    std::vector<std::complex<double>> coeffv{{2., 1.}, {4., -2}, {7., 2.}};
    // you need these hidoeus triple parenthesis here.
    constexpr std::array<std::complex<double>, 3> coeffa{
      {{2., 1.}, {4., -2}, {7., 2.}}};
    double           x1 = 12.3;
    constexpr double x2 = 12.3;
    // this will be inlined
    auto r1 = HornerConstexpr(coeffv, x1);
    // this is computed statically by the compiler
    auto r2 = HornerConstexpr(coeffa, x2);
    std::cout << "r1= " << r1 << " r2=" << r2 << std::endl;
  }
  {
    std::vector<std::complex<double>> coeffv{{2., 1.}, {4., -2}, {7., 2.}};
    // you need these hidoeus triple parenthesis here.
    std::array<std::complex<double>, 3> coeffa{{{2., 1.}, {4., -2}, {7., 2.}}};
    std::complex<double>                x1{12.3, 0};
    constexpr std::complex<double>      x2{12.3, 0};
    // this will be inlined
    auto r1 = HornerConstexpr(coeffv, x1);
    // this is computed statically by the compiler
    auto r2 = HornerConstexpr(coeffa, x2);
    std::cout << "r1= " << r1 << " r2=" << r2 << std::endl;
  }
  {
    std::vector<double>   coeffv{2., 4., 7.};
    std::array<double, 3> coeffa{2., 4., 7.};

    std::complex<double>           x1{12.3, 0};
    constexpr std::complex<double> x2 = {12.3, 0.};
    // this will be inlined
    auto r1 = HornerConstexpr(coeffv, x1);
    // this is computed statically by the compiler
    auto r2 = HornerConstexpr(coeffa, x2);
    std::cout << "r1= " << r1 << " r2=" << r2 << std::endl;
  }
}
