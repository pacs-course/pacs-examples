// HeatExchange/main.cpp
#include "GetPot"      // for command line and input file processing
#ifdef GNUPLOT          // compiled with -DGNUPLOT
#include "gnuplot-iostream.hpp" // interface with gnuplot
#endif
#include "readParameters.hpp" // for reading parameters
#include "thomas.hpp" // for Thomas algorithm. You need to have the file thomas.hpp
					  // in the include path
#include <cmath>      // (for sqrt)
#include <iostream>   // input output
#include <tuple>      // I am using tuples to pass data to gnuplot iostream
#include <vector>     // For vectors
#include <fstream>    // For file output (ofstream)
// write directive to test if I am using at least c++20
#if __cplusplus > 201703L
#include <format> // Available in C++20 and later. For formatted output
#endif
/*!
  @file main.cpp
  @brief Temperature distribution in a 1D bar.

  @details
  We solve  \f$ -T^{\prime\prime}(x)+act*(T(x)-T_e)=0, 0<x<L \f$ with
  boundary conditions \f$ T(0)=To; T^\prime(L)=0\f$

 **************************************************
  Linear finite elements
  Iterative resolution by Gauss Siedel or
  Direct solve by Thomas algorithm
 **************************************************

  Example adapted by Luca Formaggia from  a code found in
  "Simulation numerique an C++" di I. Danaila, F. Hecht e
  O. Pironneau.
 */
//! a helper function. Prints a help message
void
printHelp()
{
  std::cout
    << "USAGE: main [-h] [-v] -p parameterFile (default: parameters.pot)"
    << std::endl;
  std::cout << "-h this help" << std::endl;
  std::cout << "-v verbose output" << std::endl;
}

//! main program
int
main(int argc, char **argv)
{
  using namespace std; // avoid std::
  int    status{0};    // final program status
  bool   jsonfile = false;
  GetPot cl(argc, argv);
  if(cl.search(2, "-h", "--help")) // help required
    {
      printHelp();
      return 0;
    }
  // check if we want verbosity
  bool verbose = cl.search(1, "-v");
  // Get file with parameter values
  string filename = cl.follow("parameters.pot", "-p");// Look if -p option is there
  // if not use default parameters.pot
  auto   pos = filename.find(".json");// look for substring ".json" in filename
  if(pos != std::string::npos)
    {
      jsonfile = true;
      std::cout << "Json input file\n";
    }
  else
    {
      jsonfile = false;
      std::cout << "Getpot input file\n";
    }
  cout << "Reading parameters from " << filename << std::endl;
  parameters param;
  if(jsonfile)
    param = readParameters_json(filename, verbose);
  else
    param = readParameters(filename, verbose);

  // Check if parameter reading failed (assuming readParameters returns a default/invalid struct on failure)
  if(param.M <= 0 || param.L <= 0 || param.k <= 0) // Add more checks as appropriate
    {
      cerr << "Error: Invalid or missing parameters in input file." << endl;
      return 1;
    }

#if __cplusplus < 201703L
  // This version is perfectly fine and
  // works also if you compile with C++17+, but since C++17 you
  // may make things simpler
  // Transfer parameters to local variables, to avoid having towrite every time
  // param.xx. I use references to save memory (not really an issue here, it is
  // just to show a possible  use of references)
  const int &itermax = param.itermax; // max number of iteration for
                                      // Gauss-Siedel
  const double &toler = param.toler;  // Tolerance for stopping criterion
  // Here I use auto (remember that you need const and & if you want constant
  // references)
  const auto &L = param.L;   // Bar length
  const auto &a1 = param.a1; // First longitudinal dimension
  const auto &a2 = param.a2; //  Second longitudinal dimension
  const auto &To = param.To; // Dirichlet condition
  const auto &Te = param.Te; // External temperature (Centigrades)
  const auto &k = param.k;   // Thermal conductivity
  const auto &hc = param.hc; // Convection coefficient
  const auto &M = param.M;   // Number of grid elements
  const auto &solverType =
    param.solverType; // 1 Gauss siedel, !=1 direct method
#else
  // C++17 onwards version. This version works only with at least C++17
  // A oneliner! This is called structured bindings. It works because parameter
  // class is an aggregate!
  // This stuff is just to avoid writing param.xx all the time!

  const auto &[itermax, toler, L, a1, a2, To, Te, k, hc, M, solverType] = param;

#endif

  //! Precomputed coefficient for adimensional form of equation
  const auto act = 2. * (a1 + a2) * hc * L * L / (k * a1 * a2);

  // mesh size
  const double h = 1. / M; // I am using 1. because 1/M is an integer division since
						 // M is an integer! I want a double. You can also do
						 // static_cast<double>(M) to be more explicit. I am using auto here only
						 // to show you can do it. You can also write double h=1./M;

  // Solution vector this constructs a vector of size M+1
  std::vector<double> theta(M + 1);// dont asssume that the vector is filled with
								   // zeros. It is not guaranteed

  if(solverType == 1)
    {
      // Gauss-Seidel
      // epsilon=||x^{k+1}-x^{k}||
      // Stopping criteria epsilon<=toler
      // Gauss Siedel is initialised with a linear variation
      // of T

      for(int m = 0; m <= M; ++m)
        theta[m] = (1. - m * h) * (To - Te) / Te;

      int    iter = 0;
      double xnew, epsilon;
      // Gauss siedel iterations
      do
        {
          epsilon = 0.;

          // first M-1 row of linear system
          for(int m = 1; m < M; m++)
            {
              xnew = (theta[m - 1] + theta[m + 1]) / (2. + h * h * act);
              epsilon += (xnew - theta[m]) * (xnew - theta[m]);
              theta[m] = xnew;
            }

          // Last row
          xnew = theta[M - 1];
          epsilon += (xnew - theta[M]) * (xnew - theta[M]);
          theta[M] = xnew;

      } while((sqrt(epsilon) > toler) && (++iter < itermax));

      if(iter < itermax)
        cout << "M=" << M << "  Convergence in " << iter << " iterations"
             << endl;
      else
        {
          cerr << "NOT CONVERGING in " << itermax << " iterations "
               << "||dx||=" << sqrt(epsilon) << endl;
          status = 1;
        }
    }
  else
    {
      // Thomas algorithm. I need to build the vector a, b,c that define the
      // tridiagonal matrix
      std::vector<double> a(
        M + 1, 1.); // a vector filled with 1. using the constructor
      // to fill vectors with values you can also do like this
      std::vector<double> b;                     // an empty vector
      b.assign(M + 1, -1. / (2. + h * h * act)); // fill with values
      std::vector<double> c = b;                 // The matrix is symmetric
      b.back() = -1;                             // correction for Neumann bc
      c.front() = 0;                             // correction for Dirichlet bc c[0]=0
      std::vector<double> source(
        M + 1, 0.); // The rhs term. all zero since it is an homogeneous problem
      source.front() = (To - Te) / Te; // correction for Dirichlet bc.
      theta = apsc::thomasSolve(a, b, c, source);
    }
  // Back to physical quantities
  for(auto &t : theta)
    t = Te * (1 + t);
  // Analytic solution

  vector<double> thetaa(M + 1);
  // Analytic solution
  for(int m = 0; m <= M; m++)
    thetaa[m] =
      Te + (To - Te) * cosh(sqrt(act) * (1 - m * h)) / cosh(sqrt(act));

  // writing results with format
  // x_i u_h(x_i) u(x_i) and launch gnuplot

  std::vector<double> coor(M + 1);

  cout << "Result file: result.dat" << endl;
  ofstream f("result.dat");
  // In gnuplot lines beginning with # are comments
  // \t writes a tab
  f << "#node coordinate\tcomputed solution\texact solution" << std::endl;
#if __cplusplus > 201703L
  // I show the new format facility
  for(int m = 0; m <= M; m++)
    {
      f << std::format("{:<18.15f}\t\t{:<18.15f}\t\t{:<18.15f}\n", m * h * L,
                       theta[m], thetaa[m]);
      coor[m] = m * h * L;
    }
#else
  for(int m = 0; m <= M; m++)
    {
      f.setf(std::ios::left, std::ios::adjustfield);
      f.width(18);
      f.precision(15);
      f << m * h * L << "\t\t" << theta[m] << "\t\t" << thetaa[m] << "\n";
      coor[m] = m * h * L;
    }
#endif
    // If you have gnuplot iostream you get the plot on the screen
#ifdef GNUPLOT
  Gnuplot gp; // gnuplot iostream! Plots solution on the screen
  // It may not work on virtual machines. Take it out in that case
  // Using temporary files (another nice use of tie)
  // Comment this statement if you are not using gnuplot iostream
  // to plot the solution directly on the terminal
  gp << "plot" << gp.file1d(std::tie(coor, theta)) << "w lp lw 2 title 'uh',"
     << gp.file1d(std::tie(coor, thetaa)) << "w l lw 2 title 'uex'"
     << std::endl;
#endif
  f.close();
  return status;
}
