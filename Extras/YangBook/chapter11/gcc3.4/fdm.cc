// file fdm.cc

#include "fdm.h"

template<class T, class R>      // constructor
fdm<T,R>::fdm(T a, T b, T c, T d, int mx, int my): 
npt(mx*my), v(npt) {
  bya = a; byb = b; 
  byc = c; byd = d;
  nx = mx; ny = my; 
  hx = (byb - bya)/(nx - 1);
  hy = (byd - byc)/(ny - 1);
}

template<class T, class R> T fdm<T,R>::BandSolve() {
  T ivhxhx = 1.0/(hx*hx);
  T ivhyhy = 1.0/(hy*hy);
  T ifhxhy = 0.25/(hx*hy);
  T ithx = 0.5/hx;
  T ithy = 0.5/hy;
  int nxmone = nx - 1;
  int nymone = ny - 1;

  BandMtx<R> S(npt, nx+1,nx+1);     // stiffness matrix
  Vcr<R> w(npt);                    // load vector
  for (int j = 1; j < nymone; j++) {
    for (int i = 1; i < nxmone; i++) {
      T xi = bya + i*hx;
      T xiph = bya + (i+0.5)*hx;
      T ximh = bya + (i-0.5)*hx;
      T yj = byc + j*hy;
      T yjph = byc + (j+0.5)*hy;
      T yjmh = byc + (j-0.5)*hy;
      int rownum = j*nx + i;

      // form stiffness matrix in each row
      // corresponding to interior nodes
      S[rownum][0] =   // main diagonal
          (a11(xiph,yj) + a11(ximh,yj))*ivhxhx
        + (a22(xi,yjph) + a22(xi,yjmh))*ivhyhy
        + rcc(xi,yj);
      S[rownum][1] =   // entries above main diagonal
        - a11(xiph,yj)*ivhxhx + b1(xi,yj)*ithx;
      S[rownum][nx-1] = a12(xi-hx,yj)*ifhxhy;
      S[rownum][nx] =
        - a22(xi,yjph)*ivhyhy + b2(xi,yj)*ithy;
      S[rownum][nx+1] = - a12(xi+hx,yj)*ifhxhy;

      S[rownum][-1] =  // entries below main diagonal
        - a11(ximh,yj)*ivhxhx - b1(xi,yj)*ithx;
      S[rownum][-nx-1] = - a12(xi-hx,yj)*ifhxhy;
      S[rownum][-nx] =
        - a22(xi,yjmh)*ivhyhy - b2(xi,yj)*ithy;
      S[rownum][-nx+1] = a12(xi+hx,yj)*ifhxhy;

      w[rownum] = scf(xi,yj);  // load vector 
    }
  }

  // dealing with boundary conditions
  for (int j = 1; j < nymone; j++) {
    int rownum = j*nx;
    T yj = byc + j*hy;

    w[rownum] = bdg(bya, yj);        // on left boundary
    S[rownum][0] = 1;
    w[rownum+nxmone] = bdg(byb, yj);   // right boundary
    S[rownum+nxmone][0] = 1;
  }
  for (int i = 0; i < nx; i++) {
    T xi = bya + i*hx;
    w[i] = bdg(xi, byc);           // on bottom boundary
    S[i][0] = 1;
    w[(ny-1)*nx+i] = bdg(xi, byd);    // on top boundary
    S[(ny-1)*nx+i][0] = 1;
  }

  v = w;
  S.GaussElimPP(v);            // solve linear equations
  return (w - S*v).twonorm();  // return residual
}
