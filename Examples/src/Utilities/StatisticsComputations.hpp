/*
 * StatisticsComputations.hpp
 *
 *  Created on: Jan 9, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_UTILITIES_STATISTICSCOMPUTATIONS_HPP_
#define EXAMPLES_SRC_UTILITIES_STATISTICSCOMPUTATIONS_HPP_
#include <array>
#include <cmath>
#include <limits>
#include <tuple>

namespace apsc
{
namespace Statitics
{
  /*!
   * Class Holding data for the naive algorithm. It can be bettered adding
   * compensated summation to reduce round-off errors
   */
  class DataHolder
  {
  public:
    /*!
     * Add a new data point
     * @param x the datum
     */
    void
    add_variable(double const &x)
    {
      if(n == 0u)
        K = x;
      ++n;
      Ex += x - K;
      Ex2 += (x - K) * (x - K);
    }
    /*!
     * Remove a data point
     * @param x The datum to be removed
     */
    void
    remove_variable(double const &x)
    {
      if(n == 0u)
        return;
      --n;
      Ex -= x - K;
      Ex2 -= (x - K) * (x - K);
    }
    /*!
     * @return The sample mean
     */
    double
    get_mean() const
    {
      return K + Ex / n;
    }
    /*!
     * @return The sample variance
     */
    double
    get_variance() const
    {
      return (Ex2 - (Ex * Ex) / n) / (n - 1);
    }

  private:
    double       K = 0.;
    unsigned int n = 0u;
    double       Ex = 0;
    double       Ex2 = 0;
  };
  /*!
   * Naive algorithm to process a set of data.
   *
   * @tparam Container A container type
   * @param data A container containing the data
   * @return an array providing mean and variance
   */
  template <class Container>
  std::array<double, 2>
  shifted_data_mean_variance(Container const &data)
  {
    if(data.size() < 2)
      return {data, 0.0};
    DataHolder dh;
    for(auto x : data)
      {
        dh.add_variable(x);
      }
    // Returns an initializer list to initialize the tuple
    return {dh.get_mean(), dh.get_variance()};
  }

  /*!
   * Implements Welford recurrence relations for computing statistics
   * incrementally
   */
  class WelfordAlgorithm
  {
  public:
    /*!
     * An interesting use of an internal aggregate to store the output
     * The members are what they say.
     */
    struct AggregatedOutput
    {
      unsigned int nSamples;
      double       mean;
      double       variance;
      double       sampleVariance;
      double       skewness;
      double       kurtosis;
    };
    /*!
     * Add new data
     * @param x The datum
     */
    void
    update(const double &x)
    {
      auto n1 = count;
      ++count;
      auto delta = x - mean;
      auto delta_n = delta / count;
      auto delta_n2 = delta_n * delta_n;
      auto term1 = delta * delta_n * n1;
      mean += delta / count;
      M4 += term1 * delta_n2 * (count * count - 3 * count + 3.0) +
            6.0 * delta_n2 * M2 - 4.0 * delta_n * M3;
      M3 += term1 * delta_n * (count - 2) - 3.0 * delta_n * M2;
      M2 += delta * (x - mean);
    }
    /*!
     * returns the computed statistics.
     * @return The aggregate with all the computed statistics
     */
    AggregatedOutput
    finalize() const
    {
      // If I do not have a sufficient number of samples or
      // the samples are all equal, some of the terms may be
      // equal to Inf (or NaN if count < 2)
      // Alternative: use std::optional
      // I use initialiser list to create the output aggregate on the fly!
      // I rely on implicit conversion int->double
      return {count,
              mean,
              M2 / count,
              M2 / (count - 1.0),
              std::sqrt(static_cast<double>(count)) * M3 /
                std::pow(M2, 3. / 2.),
              (count * M4) / (M2 * M2) - 3.0};
    }

  private:
    unsigned int count = 0;
    double       mean = 0.0;
    double       M2 = 0.0;
    double       M3 = 0.0;
    double       M4 = 0.0;
  };

} // namespace Statitics
} // namespace apsc

#endif /* EXAMPLES_SRC_UTILITIES_STATISTICSCOMPUTATIONS_HPP_ */
