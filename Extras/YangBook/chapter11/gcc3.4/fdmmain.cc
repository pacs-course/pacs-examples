// file fdmmain.cc

/* since everything is in the same directory, to compile
   g++ fdmmain.cc
 */

#include "fdm.h"
#include "fdm.cc"
#include "fdmfndef.cc"

// exact solution
double exact(double x, double y) { return exp(x*y); }

int main() {
  double a = 0, b = 1, c = 0, d = 1;    // domain
  int nx = 50, ny = 50;                 // number of nodes

  fdm<double, double> pb(a, b, c, d, nx, ny);
  cout << "residual in solving linear system (two norm) = " 
       << pb.BandSolve() << endl;

  double er = 0, er2 = 0;
  for (int j=0; j < ny; j++) {
    for (int i=0; i < nx; i++) {
      double exvu = exact(a + i*pb.hx, c + j*pb.hy);
      er = max(er, abs(pb.v[i + nx*j] - exvu));
      er2 = max(er2,abs(exvu));
    }
  }
  cout << "relative FDM true error(max norm)= " 
       << er/er2 << endl;
} // end main()
