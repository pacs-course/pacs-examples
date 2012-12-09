/*!
  \file bn.cpp
  \brief Compilable file for exercise 5, point 4
  \author Daniele A. Di Pietro
  \date 8-26-2005
*/

#include <iostream>
#include "bn.hpp"

using namespace RootFinding;
using namespace std;

int main() {

  Robust r(1e-6, 100, IterativeMethod::INCREMENT, 2.e5);

  cout << r.apply(0., 1., f, df) << endl;
  cout << r << endl;

  return 0;
}
