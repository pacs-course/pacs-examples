/*!
  \file bn.cpp
  \brief Compilable file for exercise 2
*/

#include "zerofun.hpp"
#include "bn.hpp"

using namespace std;

int main() {

  int nit_bis;
  int nit_newt;

  cout << bisection(0., 1., f, 1e-8, 100,
            INCREMENT, nit_bis) << flush;
  cout << '\t' << nit_bis << endl;
  cout << newton(.1, f, df, 1e-8, 100,
         INCREMENT, nit_newt) << flush;
  cout << '\t' << nit_newt << endl;
  cout << robust(0., 1., f, df, 1e-8, 1e4, 100,
         INCREMENT, nit_bis, nit_newt)
       << flush;
  cout << '\t' << nit_bis
       << " "  << nit_newt << endl;

  return 0;
}
