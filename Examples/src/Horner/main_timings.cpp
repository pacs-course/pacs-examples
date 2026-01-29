//
//! @brief  A test of the ckass Chrono and of polynomial evaluation
//! @detail We compare the evaluation of a polygon with the standard rule
//!         against Horner's rule.

#include "chrono.hpp"
#include "horner.hpp"
#include <cmath>
#include <iostream>
#include <numeric> //for transform_reduce
#include <vector>
// Comment/uncomment next line if you dont want/want parallelization of
// transorm_reduce
#ifdef PARALLELEXEC
#include <execution>
#endif

int
main()
{
  using namespace std;
  using namespace Timings;

  vector<double> a;
  int            n;
  cout << "give me polynomial degree" << endl;
  cout << "=>";
  cin >> n;
  a.reserve(n + 1);
  cout << "Coefficients are computed automatically" << endl;
  for(int i = 0; i <= n; ++i)
    a.emplace_back(2 * std::sin(2.0 * i));
  /*
  cout<<"Give me coefficients a0, a1..."<<endl;
  for(int i=0;i<=n;++i){
    double temp;
    cout<<"a["<<i<<"]=";
    cin>>temp;
    cout<<endl;
    a.push_back(temp);
  }
  */
  double startx = 0.00;
  double endx = 1.00;
  double interval = 0.0001;
  int    numinterval =
    static_cast<int>((endx - startx) / interval); // truncated if not integer!
  cout << "Computing " << numinterval << " evaluation of polynomial"
       << " with standard formula" << endl;

  vector<double> points;
  points.reserve(numinterval + 1);
  double x = startx;
  // create a vector with some values
  for(int i = 0; i <= numinterval; ++i)
    {
      points.emplace_back(x);
      x += interval;
    }
  Chrono timer; // start clock
  timer.start();
  auto sol1 = evaluatePoly(points, a, &eval);
  timer.stop(); // stop clock
  auto timeStandard = timer.wallTime();
  cout << endl;

  cout << timer << endl;

  cout << "Computing " << numinterval << " evaluation of the polynomial with"
       << " Horner's rule";
#ifdef PARALLELEXEC
  cout << " (parallel version of transform_reduce)";
#endif
  cout << endl;
  timer.start();
  auto sol2 = evaluatePoly(points, a, &horner);
  timer.stop();
  cout << endl;
  auto timeHorner = timer.wallTime();
  cout << timer << endl;
  cout << "Computing " << numinterval << " evaluation of the polynomial with"
       << " Horner's rule and range based algorithms" << endl;
  timer.start();
  auto sol3 = evaluatePoly(points, a, &horner_range);
  timer.stop(); // stop clock
  auto timeHornerRange = timer.wallTime();
  cout << timeHornerRange << endl;

  // compute || sol1 - sol2 ||^2_2 using std::transform_reduce
#ifdef PARALLELEXEC // parallel version
  auto diff = std::transform_reduce(
    std::execution::par, sol1.begin(), sol1.end(), sol2.begin(), 0.0,
    std::plus<>(),
    [](auto const &x, auto const &y) { return (x - y) * (x - y); });
  auto diff2 = std::transform_reduce(
    std::execution::par, sol1.begin(), sol1.end(), sol3.begin(), 0.0,
    std::plus<>(),
    [](auto const &x, auto const &y) { return (x - y) * (x - y); });
#else
  auto diff = std::transform_reduce(
    sol1.begin(), sol1.end(), sol2.begin(), 0.0, std::plus<>(),
    [](auto const &x, auto const &y) { return (x - y) * (x - y); });
  auto diff2 = std::transform_reduce(
    sol1.begin(), sol1.end(), sol3.begin(), 0.0, std::plus<>(),
    [](auto const &x, auto const &y) { return (x - y) * (x - y); });
#endif
  cout << "||sol1-sol2||_2/N=" << std::sqrt(diff) / sol1.size()
       << " (it should be close to zero)" << endl;
  cout << "||sol1-sol3||_2/N=" << std::sqrt(diff2) / sol1.size()
       << " (it should be close to zero)" << endl;
  std::string message = "Speedup standard/Horner ";
#ifdef PARALLELEXEC
  message += "(parallel version of transform_reduce) ";
#endif
  cout << message << timeStandard / timeHorner << endl;
  cout << "Speedup standard/HornerRange " << timeStandard / timeHornerRange
       << endl;
  return 0;
} // end of main()
