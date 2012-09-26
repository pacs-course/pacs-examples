/*!
  \file bn.cpp
  \brief Compilable file
*/

#include <iostream>
#include "src/robust.hpp"
#include "src/bisection.hpp"
#include "src/newton.hpp"
#include "bn.hpp"

using namespace RootFinding;
using namespace std;

int main()
{
  Bisection b(1e-6, 100, INCREMENT);
  Newton n(1e-6, 100);
  Robust r(1e-6, 100, INCREMENT, 2.e5);

  cout << "Bisection: root " << b.apply(0., 1., f) << flush;
  cout << '\t' << "nit: "<< b.nit() << endl;
  cout << "Newton(x0=0.1): root " << n.apply(.1, f, df) << flush;
  cout << '\t' << "nit: "<< n.nit() << endl;
  cout << "Robust: root " << r.apply(0., 1., f, df) << flush;
  cout << '\t' << "C nit "  << r.nitcoarse() << flush;
  cout << "  F nit " << r.nitfine() << endl;
  cout << "Newton(x0=0.): root " << n.apply(0., f, df) << flush;
  cout << '\t' << "nit: "<< n.nit() << endl;
  return 0;
}
