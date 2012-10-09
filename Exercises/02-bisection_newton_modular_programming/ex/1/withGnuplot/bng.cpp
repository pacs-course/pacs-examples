/*!
  \file bn.cpp
  \brief Compilable file for exercise 2
*/

#include "zerofung.hpp"
#include "bng.hpp"

using namespace std;

int main() {

  int nit_bis;
  int nit_newt;
  const double sol_ex = sqrt(0.5);
  const string file_name = "data";

  cout << robust(0., 1., f, df, 1e-8, 1e4, 100,
         INCREMENT, nit_bis, nit_newt, sol_ex, file_name)
       << flush;
  cout << '\t' << nit_bis
       << " "  << nit_newt << endl;

  system("gnuplot print_data");
 
  return 0;
}
