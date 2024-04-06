/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012 and modified by
 * Luca Formaggia, 2012.
 *
 * (C) Copyright Nicolai M. Josuttis 2012, Luca Formaggia, 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
/*!
  @file dist1.cpp
  @brief An example on use of random number generation and distributions.
  @note  Additional comments and a few additions by Luca Formaggia.
 */
#include "sampling.hpp"
#ifndef NO_GNUPLOT
#include "gnuplot-iostream.hpp"
#endif
#include "StatisticsComputations.hpp"
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
//! A helper function to print the distribution
/*!
 *  It prints a simplified histogram for distribution d using engine e
 *  if file=true it will also produce a file with name file.dat with the
 *  values.
 *  By default it does not produce any file
 *  \param d  a distribution
 *  \param name A name for output and file
 *  \param file true is you want a file with generated numbers (def. false)
 *  \param hist true if you want a poor man histogram on the screen (true)
 */
template <typename Distr, typename Eng>
void
distr(Distr &d, Eng &e, const std::string &name, bool file = false,
      bool hist = true)
{
  // The engine produce integer numbers whose width depends on the
  // engine (and possibly the chosen engine template argummnts)
  // But we can extract the type from the engine itself if we need it with
  // typedef typename Eng::result_type eng_result_type;
  // The same for the result type of the distribution:
  // typedef typename Distr::result_type distr_result_type;
  // Remember to use typename since we have template parameter dependent types
  //
  // Number of samples
  constexpr int N = 150000;
  std::ofstream outfile;
  if(file)
    {
      std::string filename = name + std::string(".dat");
      outfile.open(filename);
      outfile << N << "\n";
    }
  // print min, max and four example values
  std::cout << name << ":" << std::endl;
  std::cout << "- min():  " << d.min() << std::endl;
  std::cout << "- max():  " << d.max() << std::endl;
  std::cout << "- some values: " << d(e) << ' ' << d(e) << ' ' << d(e) << ' '
            << d(e) << std::endl;
  std::vector<typename Distr::result_type> values;
  values.reserve(N);
  for(int i = 0; i < N; ++i)
    {
      values.emplace_back(d(e));
      if(file)
        outfile << values.back() << "\n";
    }
  //@note To reduce floating point errors, particularly in the computation of
  //high moments, I order the data
  std::sort(values.begin(), values.end(),
            [](auto a, auto b) { return std::abs(a) < std::abs(b); });
  auto minValue = *std::min_element(values.begin(), values.end());
  auto maxValue = *std::max_element(values.begin(), values.end());
  auto deltaValues = maxValue - minValue;
  apsc::Statitics::WelfordAlgorithm stat;
  for(auto x : values)
    stat.update(x);
  std::cout << "Sample statistics:\n";
  auto output = stat.finalize();
  std::cout << "N. Samples :" << output.nSamples << std::endl;
  std::cout << "Mean       :" << output.mean << std::endl;
  std::cout << "Variance   :" << output.variance << std::endl;
  std::cout << "Sample Var.:" << output.sampleVariance << std::endl;
  std::cout << "Skewness   :" << output.skewness << std::endl;
  std::cout << "Ex.Kurtosis:" << output.kurtosis << std::endl;
  if(file)
    outfile.close();

  if(hist)
    {
      int nBars =
        25; // numbers of bars in the histogram (if values are floating points)
      if(std::is_integral_v<typename Distr::result_type>)
        {
          ++deltaValues;
          nBars = deltaValues;
        }
      std::vector<std::string> barLegend;
      barLegend.resize(nBars);
      std::vector<typename Distr::result_type> intervalMidPoint;
      intervalMidPoint.resize(nBars);
      auto h = deltaValues / nBars;
      for(int i = 0; i < nBars; ++i)
        {
          intervalMidPoint[i] = minValue + i * h + h / 2;
          std::ostringstream ss;
          ss << intervalMidPoint[i];
          barLegend[i] = ss.str();
        }
      // Make an istogram of the generated values
      // For simplicity we convert them to integers
      // As key we use long long to account for big numbers

      std::vector<int> valuecounter;
      valuecounter.resize(nBars, 0);
      for(int i = 0; i < N; ++i)
        {
          auto value = values[i];
          int  pos = (value - minValue) / h;
          pos = std::min(pos, nBars - 1);
          valuecounter[pos]++; // this works!
        }
        // print the resulting distribution on the screen
#ifdef NO_GNUPLOT
      std::cout << "====" << std::endl;
      for(int i = 0; i < nBars; ++i)
        {
          std::cout << std::setw(3) << barLegend[i] << ": " << valuecounter[i]
                    << std::endl;
        }
#else
      Gnuplot                                      gp;
      std::vector<std::tuple<std::string, double>> gdata;
      for(int i = 0; i < nBars; ++i)
        gdata.emplace_back(std::make_tuple(barLegend[i], valuecounter[i]));

      gp
        << "set style data histograms\n set style fill solid\n set xtics rotate"
        << std::endl;
      gp << "plot '-' using 2:xtic(1) title '" << name.c_str() << "'"
         << std::endl;
      // gp.send1d(std::make_tuple(barLegend,valuecounter));
      gp.send1d(gdata);
      std::cout << "Type anything to continue" << std::endl;
      std::string nothing;
      std::cin >> nothing;
#endif
    }
  std::cout << "====" << std::endl;
  std::cout << std::endl;
}
//! Example printing several distributions.
/*!
  All distribution have a defualt constructor which sets the parameter
  distribution to default values. Look at a reference manual to have a
  complete list of the possible arguments.
 */
int
main()
{
  // Set it to true if you also want the files
  bool               file = true;
  std::random_device re;
  // Try the random engine
  auto myseed = re();
  // unsigned int myseed=123456789;
  std::cout << "The seed is " << myseed << std::endl;
  // We use the knuth engine
  std::knuth_b e(myseed);
  // uncomment if you want the random device
  // std::random_device e;

  // Note the <>. It is compulsory, since it is a
  // full specialization of a class template
  std::cout << "Uniform real distribution in the interval [0,10), Mean=5, Var="
            << 100 / 12. << "Skew= 0, Ex. Kurt.=" << -6. / 5. << "\n";
  std::uniform_real_distribution ud{0., 10.};
  distr(ud, e, "uniform-real-distribution-0-10", file);

  // Default is mean 0 and standard deviation 1
  std::cout << "Normal distribution mean=0, Var=1, Skew=0 Ex Kurt =0\n";
  std::normal_distribution nd;
  distr(nd, e, "normal-distribution-mean-0-stdev-1", file);

  // Mean 1  Std dev 2.0
  std::cout << "Normal distribution mean=1, Var=4, Skew=0, Ex. Kurt.=0\n";
  std::normal_distribution nd2{1.0, 2.0};
  distr(nd2, e, "normal-distribution-mean-1-stdev-2", file);

  std::cout << "Exponential distribution mean=1, Var=1, Skew=2, Ex Kurt= 6\n";
  std::exponential_distribution ed;
  distr(ed, e, "exponential-distribution-lambda-1", file);

  std::cout << "Gamma distribution k=2, theta=1, mean=2, Var=2, Skew="
            << 2. / std::sqrt(2.0) << ", Ex Kurt= 3\n";
  std::gamma_distribution gd{2., 1.};
  distr(gd, e, "gamma-distribution-k2-t1", file);

  std::cout << "LogNormal distribution mu=3, s=2, mean=" << std::exp(5.)
            << ", Var=" << (std::exp(4.) - 1) * std::exp(10)
            << ", Skew=" << (std::exp(4.0) + 2) * std::sqrt(std::exp(4) - 1)
            << ", Ex Kurt"
            << std::exp(16.) + 2 * std::exp(12.) + 3 * std::exp(8.) - 6.0
            << "3\n";
  std::lognormal_distribution lnd{3., 2.};
  distr(lnd, e, "lognormal-distribution-m3-s2", file);

  auto sigmaw = std::tgamma(3.) - std::tgamma(2.);
  std::cout << "Weibull Distribution k=1, l=1, mean=" << std::tgamma(2.0)
            << ", Var=" << sigmaw * sigmaw << ", Skew="
            << (std::tgamma(4.0) - 3 * sigmaw * sigmaw - 1) /
                 (sigmaw * sigmaw * sigmaw)
            << std::endl;
  std::weibull_distribution wd{1., 1.};
  distr(wd, e, "weibull-distribution-k1-l1", file);

  // This produces a very "elongated" distribution
  // so I am not printing on the scrren
  std::student_t_distribution sd{2.0};
  std::cout << "Student T Distribution nu=2, mean=0, Var=Inf, Skew=Undef, Ex "
               "Kurt=Undef";
  distr(sd, e, "student-t-distribution", true, true);
  // Now a discrete distribution. The constructor
  // may take different forms, in this case we give the
  // weights for the values
  std::cout << "Discrete Distribution in [0,6) with weight 1 3 6 1 1 10\n";
  std::discrete_distribution<int> ddist{1., 3., 6., 1., 1., 10.};
  distr(ddist, e, "discrete-distribution", true, true);
  //
  // **************** THE SHUFFLING PART
  // To show it works on any container I usa a map
  std::map<int, std::string> dict = {
    {1, "Mario"},     {2, "Giulio"},   {3, "Maria"}, {4, "John"},
    {5, "Matilde"},   {6, "Paola"},    {7, "Anna"},  {9, "Giovanni"},
    {10, "Beatrice"}, {11, "Juliette"}};
  // Extract 5 at random
  auto sampled = apsc::sampleContainer(dict, 5u);
  std::cout << "Extracted sample\n";
  for(auto &i : sampled)
    std::cout << "(" << i.first << ", " << i.second << "); ";
  std::cout << std::endl;
  // now shuffle
  // It is a map! Element are not swappable
  // Move to a vector
  std::vector<std::pair<int, std::string>> sampleVec{sampled.begin(),
                                                     sampled.end()};
  //! We can move it, we do not need it afterwards
  auto shuffled = apsc::shuffleContainer(std::move(sampleVec));
  std::cout << "Shuffled sample\n";
  for(auto &i : shuffled)
    std::cout << "(" << i.first << ", " << i.second << "); ";
  std::cout << std::endl;
}
