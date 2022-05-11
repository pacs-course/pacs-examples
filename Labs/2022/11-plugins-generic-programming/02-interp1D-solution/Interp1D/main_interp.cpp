/*
 * main_interp.cpp
 *
 *  Created on: Apr 14, 2020
 *      Author: forma
 */

#include "interp1D.hpp"
#include "interp1D_util.hpp"
#include <functional>
#include <iostream>

int
main()
{
  std::vector<std::array<double, 2> > v = {
    {0.1, 2.0},    {0.3, 3.0},  {0.5, 5.0}, {0.55, 5.5},
    {0.65, -10.0}, {0.7, -8.0}, {0.8, 4.0}};
  std::vector<double> keys = {0, 0.1, 0.35, 0.7, 0.75, 0.8, 0.9};
  // print it out:
  std::cout<<"Interpolation points\nx\ty\n";
  for(auto i : v)
    std::cout << i[0] << "\t" << i[1] << std::endl;
  std::cout<<"interpolate!"<<std::endl;
  for(auto key : keys)
    {
      double xin = apsc::interp1D(v, key);
      std::cout << "Value at " << key << " equal to " << xin << std::endl;
    }
  // To check if it works also with other comparison opts
  std::cout << "\nreverse ordering\nx\ty\n";
  std::vector<std::array<double, 2> > vr{v.rbegin(), v.rend()}; // reverse vecto
  // print it out:
  for(auto i : vr)
    std::cout << i[0] << "\t" << i[1] << std::endl;
  // now is from largest to smaller. less<T> is inconsistent: I have to use
  // std::greater
  for(auto key : keys)
    {
      double xin = apsc::interp1D(vr, key, std::greater<double>());
      std::cout << "Value at " << key << " equal to " << xin << std::endl;
    }
  // Now the version that takes two vectors separately for keys (nodes) and
  // values

  std::vector<double> nodes;
  std::vector<double> values;
  // fill with same nodes and values as before
  nodes.reserve(v.size());
  for(auto i : v)
    nodes.emplace_back(i[0]);
  values.reserve(v.size());
  for(auto i : v)
    values.emplace_back(i[1]);
  std::cout << "\nNow the version taking two vectors\n";
  for(auto key : keys)
    {
      double xin = apsc::interp1D(nodes, values, key);
      std::cout << "Value at " << key << " equal to " << xin << std::endl;
    }
}
