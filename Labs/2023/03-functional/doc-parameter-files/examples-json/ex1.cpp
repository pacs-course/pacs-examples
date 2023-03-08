// need to install the json submodule, after can compile with
// g++ ex1.cpp -I../../../../../Examples/include
#include <fstream>
#include <iostream>

#include "json.hpp"

using json = nlohmann::json;

int main() {
  std::ifstream f("data.json");
  json data = json::parse(f);
  const double a = data["domain"].value("a", 0.0);
  const double b = data["domain"].value("b", 1.0);
  const unsigned int n_intervals = data["mesh"].value("n_intervals", 10);
  std::cout << a << " " << b << " " << n_intervals << std::endl;
  return 0;
}
