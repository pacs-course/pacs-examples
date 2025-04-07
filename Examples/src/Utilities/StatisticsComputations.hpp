/*
 * StatisticsComputations.hpp
 *
 *  Created on: Jan 9, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_UTILITIES_STATISTICSCOMPUTATIONS_HPP_
#define EXAMPLES_SRC_UTILITIES_STATISTICSCOMPUTATIONS_HPP_
#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <tuple>

namespace apsc
{
namespace Statistics
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
      long double  mean;
      long double  variance;
      long double  sampleVariance;
      long double  skewness;
      long double  kurtosis;
      long double  uncorrectedKurtosis;
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
      M2 += delta * (x - mean);
      M3 += term1 * delta_n * (count - 2) - 3.0 * delta_n * M2;
      M4 += term1 * delta_n2 * (count * count - 3 * count + 3.0) +
            6.0 * delta_n2 * M2 - 4.0 * delta_n * M3;
    }
    /*!
     * Returns the computed statistics.
     * @return The aggregate with all the computed statistics
     */
    AggregatedOutput
    finalize() const
    {
      // If the sample size is too small, return default values
      if(count < 4)
        {
          return {count, mean, M2 / count, M2 / (count - 1.0), 0.0, 0.0, 0.0};
        }

      // Compute uncorrected excess kurtosis
      long double uncorrectedKurtosis = (count * M4) / (M2 * M2) - 3.0;

      // Apply bias correction for kurtosis
      long double correctedKurtosis =
        (static_cast<long double>(count - 1.0) /
         (static_cast<long double>(count - 2.0) *
          static_cast<long double>(count - 3.0))) *
        ((count + 1.0) * uncorrectedKurtosis + 6.0);

      // Compute skewness
      long double skewness =
        std::sqrt(static_cast<long double>(count)) * M3 / std::pow(M2, 1.5);

      // Return the aggregated statistics
      return {count,
              mean,
              M2 / count,
              M2 / (count - 1.0),
              skewness,
              correctedKurtosis,
              uncorrectedKurtosis};
    }

  private:
    unsigned int count = 0;
    long double  mean = 0.0;
    long double  M2 = 0.0;
    long double  M3 = 0.0;
    long double  M4 = 0.0;
  };
  /*!
   * Normalize the data in place. The data is normalized to the range [0,1]
   * @tparam Range A container type
   * @param data The data to be normalized
   * @return a pair with the min and max values
   */
  template <typename Range>
  auto
  normalizeInPlace(Range &data)
  {
    auto        minmax_prt = std::minmax_element(data.begin(), data.end());
    std::pair   minmax = {*minmax_prt.first, *minmax_prt.second};
    auto const &min = minmax.first;
    auto const &max = minmax.second;
    auto        delta = max - min;
    if(delta == 0)
      return minmax;
    for(auto &x : data)
      {
        x = (x - min) / delta;
      }
    return minmax;
  }

  /*!
   * Reset the statistics in place, reverting the effect of the normalization
   * @tparam Range A container type
   * @param stat The statistics to be reset
   * @param minmax The min and max values of the data in a std::pair
   */
  template <typename T>
  void
  resetStatisticsInPlace(WelfordAlgorithm::AggregatedOutput &stat,
                         std::pair<T, T> const              &minmax)
  {
    // Reset the statistics reverting the normalization
    auto delta = minmax.second - minmax.first;
    if(delta == 0)
      return;
    stat.mean = minmax.first + stat.mean * delta;
    stat.variance *= delta * delta;
    stat.sampleVariance *= delta * delta;
  }
} // namespace Statistics
} // namespace apsc

#endif /* EXAMPLES_SRC_UTILITIES_STATISTICSCOMPUTATIONS_HPP_ */
