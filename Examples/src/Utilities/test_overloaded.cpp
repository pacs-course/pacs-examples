/*
 * test_overloaded.cpp
 *
 *  Created on: Apr 21, 2022
 *      Author: forma
 */
#include <iomanip>
#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include "overloaded.hpp"
int main()
{
  using var_t = std::variant<int, long, double, std::string>;
  std::vector<var_t> vec{10, 15l, 1.5, "hello"};
  auto visitor=apsc::overloaded {
          [](auto arg) { std::cout << arg << ' '; },
          [](double arg) { std::cout << std::fixed << arg << ' '; },
          [](const std::string& arg) { std::cout << std::quoted(arg) << ' '; }
      };

  std::cout<<"Visiting a vector of variants\n";
  for(auto& v: vec) {
   std::visit(visitor, v);
      }
  std::cout<<std::endl;
}


