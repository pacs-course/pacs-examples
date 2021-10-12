/* Calcolo della distribuzione di temperatura
   in una aletta di raffreddamento

   ********* caso stazionario *******************
     -u''(x)+act*u(x)=0, 0<x<1
     u(0)=uo; u'(1)=0
   **************************************************
   discretizzazione agli elementi finiti lineari
   Risoluzione mediante Gauss Siedel.
   **************************************************

   Esempio preso da "simulazion numerique an C++" di I. Danaila, F. Hecht e
   O. Pironneau. Adattato e modificato da L. Formaggia 2005
*/

#include "chrono.hpp"
#include <cmath>    // operazioni matematiche (sqrt)
#include <fstream>  // input output su file
#include <iostream> // input output
#include <vector>   // Vettori della STL

using namespace std; // Non occorre usare std::

/* Variabili globali:
   variabili definite all'esterno del blocco "main"
   sono variabili globali per il programma.
   In programmi piu` complessi l' uso di variabili globali per
   definire dei parametri non e` consigliato. E` meglio definire una classe o un
   struct apposita.
*/
const int    MMAX = 501, itermax = 1000000;
const double toler = 1e-8;
const double L = 40., a1 = 4., a2 = 50.;
const double To = 46., Te = 20.;
const double k = 0.164, hc = 1.e-6 * 200.;
const double act = 2. * (a1 + a2) * hc * L * L / (k * a1 * a2);

void solve(int const itermax, int const M, double const &h,
           vector<double> &theta);

int
main()
{
  Timings::Chrono myclock;
  myclock.start();
  // Construzione della griglia

  int          M;
  std::fstream ifile("dati");

  ifile >> M;
  ifile.close();

  double h = 1. / M;

  // Vettore soluzione: incognite piu` dato al bordo
  //  vector<double> theta(M+1);
  // ERRORE!!
  vector<double> theta;
  // Se non si vuole usare vector questo e` il modo
  // di costruire un array dinamico in C++
  // double * theta = new double[M+1];

  // vector<double> theta(M+1);

  // Inizializzazione di Gauss Siedel dist. lineare

  theta[0] = (To - Te) / Te; // condizione a x=0
  for(int m = 1; m <= M; m++)
    theta[m] = (1. - m * h) * (To - Te) / Te;

  // Gauss-Seidel
  // epsilon=||x^{k+1}-x^{k}||
  // Criterio di arresto: epsilon<=toler

  solve(itermax, M, h, theta);

  // Solutione analitica

  vector<double> thetaa(M + 1);
  for(int m = 0; m <= M; m++)
    thetaa[m] =
      Te + (To - Te) * cosh(sqrt(act) * (1 - m * h)) / cosh(sqrt(act));

  // Scrittura su file dei risultati nel formato
  // x_i u_h(x_i) u(x_i)

  cout << "file dei risultati: result.dat" << endl;
  ofstream f("result.dat");
  for(int m = 0; m <= M; m++)
    f << m * h * L << "\t" << Te * (1. + theta[m]) << "\t" << thetaa[m] << endl;

  f.close();
  // Se non si usa vector<> occorre liberare la memoria!
  // delete[] theta;
  myclock.stop();
  std::cout << myclock << std::endl;

  return 0;
}

void
solve(int const itermax, int const M, double const &h, vector<double> &theta)
{
  int    iter = 0;
  double epsilon(0);
  do
    {
      double xnew;
      epsilon = 0;
      // prime M-1 righe del sistema lineare
      for(int m = 1; m < M; m++)
        {
          xnew = (theta[m - 1] + theta[m + 1]) / (2. + h * h * act);
          epsilon += (xnew - theta[m]) * (xnew - theta[m]);
          theta[m] = xnew;
        }

      // ultima riga
      xnew = theta[M - 1];
      epsilon += (xnew - theta[M]) * (xnew - theta[M]);
      theta[M] = xnew;

      iter = iter + 1;
    }
  while((sqrt(epsilon) > toler) && (iter < itermax));

  if(iter < itermax)
    cout << "M=" << M << "  Convergenza in " << iter << " iterationi" << endl;
  else
    cout << "NON CONVERGE. " << itermax << " iterazioni "
         << "||dx||=" << sqrt(epsilon) << endl;
}
