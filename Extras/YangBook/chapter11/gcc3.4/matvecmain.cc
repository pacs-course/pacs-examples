#include "matvec.h"

// this is a test for the matrix vector library

main() {

cout.setf(ios::scientific, ios::floatfield);
//cout.setf(ios::fixed, ios::floatfield);
cout.precision(3);

// first test for real number arithmetic
{   
  cout << " ******** Testing for real matrices and vectors \n";

  int n = 20, m = 20;

  FullMtx<double> mat1(n,m);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
        mat1[i][j] =  n/(abs(i - j) + 1.0);
    }
  }

  Vcr<double> vec1(n) ;
  for (int i = 0; i < n; i++) vec1[i] = i;

//  FullMtx<double> mat2 = mat1;
//  Vcr<double> vec2 = 0.5*vec1 - vec1;

//  cout << "inner prod of vec1 and vec 2 = " << dot(vec1,vec2) << "\n";
//  cout << "L-2norm  of v2 = " << vec2.twonorm() << "\n";
//  cout << "maxnorm  of v2 = " << vec2.maxnorm() << "\n";

  Vcr<double> vec3 = mat1*vec1;
  mat1.GaussElim(vec3);
  cout << "error in Gauss Elim =" << (vec3 - vec1).twonorm() << "\n";

  vec3 = mat1*vec1;
  mat1.GaussElimPP2(vec3);
  cout << "error in GaussElimPP2 =  " << (vec3 - vec1).twonorm() << "\n";

  vec3 = mat1*vec1;
  mat1.GaussElimCP(vec3);
  cout << "error in GaussElimCP =  " << (vec3 - vec1).twonorm() << "\n";

  int prec = 1;  
  int iter = n;
  double eps = 1.0e-14;
  Vcr<double> vec2(n);
  int ret =  mat1.CG(vec2, mat1*vec1,eps,iter,prec);
  if (ret == 0) cout << "CG returned successfully\n";
//  cout << "true solutionn is: " << vec1 << "   ";
//  cout << "CGsolution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in CG= " << eps << " \n " ;
  cout << "Ture error in CG= " << (vec2-vec1).maxnorm() << "\n" ;

  iter = n;
  eps = 1.0e-14;
  vec2.reset();
  ret =  mat1.GMRES(vec2, mat1*vec1,eps,iter,prec,n);
  if (ret == 0) cout << "GMRES returned successfully\n";
//  cout << "true solutionn is: " << vec1 << " ";
//  cout << "GMRES solution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in GMRES = " << eps << " \n " ;
  cout << "Ture error in GMRES = " << (vec2-vec1).maxnorm() << "\n" ;

  cout << "******** Banded Matrix  ******** \n";
  BandMtx<double> bd1(n,n-1,n-1,mat1);

  vec3 = bd1*vec1;
  bd1.GaussElim(vec3);
  cout << "error in BandGaussElim =  " << (vec3 - vec1).twonorm() << "\n";
  vec3 = bd1*vec1;
  bd1.GaussElimPP(vec3);
  cout << "error in BandGaussElimPP =  " << (vec3 - vec1).twonorm() << "\n";
   
  iter = n;
  eps = 1.0e-14;
  vec2.reset();
  ret =  bd1.CG(vec2, mat1*vec1,eps,iter,prec);
  if (ret == 0) cout << "CG returned successfully\n";
//  cout << "true solutionn is: " << vec1 << " ";
//  cout << "CG solution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in CG= " << eps << "  " ;
  cout << "Ture error in CG= " << (vec2-vec1).maxnorm() << "\n\n" ;

  iter = n;
  eps = 1.0e-14;
  vec2.reset();
  ret =  bd1.GMRES(vec2, mat1*vec1,eps,iter,prec,n);
  if (ret == 0) cout << "GMRES returned successfully\n";
//  cout << "true solutionn is: " << vec1 << " ";
//  cout << "GMRES solution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in GMRES= " << eps << "  " ;
  cout << "Ture error in GMRES= " << (vec2-vec1).maxnorm() << "\n\n" ;

  cout << "******** Sparse Matrix  ******** \n";
  SparseMtx<double> sm1(n,n*n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      sm1[i*n + j] = mat1[i][j];
      sm1.getclm(i*n + j) = j;
    }
    sm1.getfnz(i) = i*n; 
  }
  sm1.getfnz(n) = n*n;

  iter = n;
  eps = 1.0e-14;
  vec2.reset();
  ret =  sm1.CG(vec2, sm1*vec1,eps,iter,prec);
  if (ret == 0) cout << "CG returned successfully\n";
//  cout << "true solutionn is: " << vec1 << " ";
//  cout << "CG solution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in CG = " << eps << "  " ;
  cout << "Ture error in CG= " << (vec2-vec1).maxnorm() << "\n\n" ;

  iter = n;
  eps = 1.0e-14;
  vec2.reset();
  ret =  sm1.GMRES(vec2, sm1*vec1,eps,iter,prec,n);
  if (ret == 0) cout << "GMRES returned successfully\n";
//  cout << "true solutionn is: " << vec1 << " ";
//  cout << "GMRES solution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in GMRES = " << eps << "  " ;
  cout << "Ture error in GMRES= " << (vec2-vec1).maxnorm() << "\n\n" ;
}


// then test for complex number arithmetic
{   
  cout << " ******** Testing for complex matrices and vectors \n";

  int n = 200, m = 200;

  FullMtx<complex<double> > mat1(n,m);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
       complex<double> ii = (i,i);
       complex<double> jj = (j,j);
        mat1[i][j] =  (n,i)/(abs(ii - jj) + (1,1)) + cos(ii+jj);
    }
  }

  Vcr< complex<double> > vec1(n) ;
  for (int i = 0; i < n; i++) vec1[i] = (i,i);

  Vcr<complex<double> > vec3 = mat1*vec1;
  mat1.GaussElim(vec3);
  cout << "error in Gauss Elim =" << (vec3 - vec1).twonorm() << "\n";

  vec3 = mat1*vec1;
  mat1.GaussElimPP2(vec3);
  cout << "error in GaussElimPP2 =  " << (vec3 - vec1).twonorm() << "\n";

  vec3 = mat1*vec1;
  mat1.GaussElimCP(vec3);
  cout << "error in GaussElimCP =  " << (vec3 - vec1).twonorm() << "\n";

  int prec = 1;  
  int iter = n;
  double eps = 1.0e-14;
  Vcr<complex<double> > vec2(n);
  int ret =  mat1.CG(vec2, mat1*vec1,eps,iter,prec);
  if (ret == 0) cout << "CG returned successfully\n";
//  cout << "true solutionn is: " << vec1 << "   ";
//  cout << "CGsolution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in CG= " << eps << " \n " ;
  cout << "Ture error in CG= " << (vec2-vec1).maxnorm() << "\n" ;

  iter = n;
  eps = 1.0e-14;
  vec2.reset();
  ret =  mat1.GMRES(vec2, mat1*vec1,eps,iter,prec,n);
  if (ret == 0) cout << "GMRES returned successfully\n";
//  cout << "true solutionn is: " << vec1 << " ";
//  cout << "GMRES solution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in GMRES = " << eps << " \n " ;
  cout << "Ture error in GMRES = " << (vec2-vec1).maxnorm() << "\n" ;

  cout << "******** Banded Matrix  ******** \n";
  BandMtx< complex< double> > bd1(n,n-1,n-1,mat1);

  vec3 = bd1*vec1;
  bd1.GaussElim(vec3);
  cout << "error in BandGaussElim =  " << (vec3 - vec1).twonorm() << "\n";
  vec3 = bd1*vec1;
  bd1.GaussElimPP(vec3);
  cout << "error in BandGaussElimPP =  " << (vec3 - vec1).twonorm() << "\n";
   
  iter = n;
  eps = 1.0e-14;
  vec2.reset();
  ret =  bd1.CG(vec2, mat1*vec1,eps,iter,prec);
  if (ret == 0) cout << "CG returned successfully\n";
//  cout << "true solutionn is: " << vec1 << " ";
//  cout << "CG solution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in CG= " << eps << "  " ;
  cout << "Ture error in CG= " << (vec2-vec1).maxnorm() << "\n\n" ;

  iter = n;
  eps = 1.0e-14;
  vec2.reset();
  ret =  bd1.GMRES(vec2, mat1*vec1,eps,iter,prec,n);
  if (ret == 0) cout << "GMRES returned successfully\n";
//  cout << "true solutionn is: " << vec1 << " ";
//  cout << "GMRES solution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in GMRES= " << eps << "  " ;
  cout << "Ture error in GMRES= " << (vec2-vec1).maxnorm() << "\n\n" ;

  cout << "******** Sparse Matrix  ******** \n";
  SparseMtx<complex<double> > sm1(n,n*n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      sm1[i*n + j] = mat1[i][j];
      sm1.getclm(i*n + j) = j;
    }
    sm1.getfnz(i) = i*n; 
  }
  sm1.getfnz(n) = n*n;

  iter = n;
  eps = 1.0e-14;
  vec2.reset();
  ret =  sm1.CG(vec2, sm1*vec1,eps,iter,prec);
  if (ret == 0) cout << "CG returned successfully\n";
//  cout << "true solutionn is: " << vec1 << " ";
//  cout << "CG solution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in CG = " << eps << "  " ;
  cout << "Ture error in CG= " << (vec2-vec1).maxnorm() << "\n\n" ;

  iter = n;
  eps = 1.0e-14;
  vec2.reset();
  ret =  sm1.GMRES(vec2, sm1*vec1,eps,iter,prec,n);
  if (ret == 0) cout << "GMRES returned successfully\n";
//  cout << "true solutionn is: " << vec1 << " ";
//  cout << "GMRES solution =  " << vec2 << "\n";
  cout << iter << " iterations used. " ;
  cout << "Residual in GMRES = " << eps << "  " ;
  cout << "Ture error in GMRES= " << (vec2-vec1).maxnorm() << "\n\n" ;

}


}
