#ifndef RKF_HPP
#define RKF_HPP

#include "ButcherRKF.hpp"
#include "RKFTraits.hpp"

#include <GetPot>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

/// Struct holding the results of the RKF solver.
template <class ProblemType>
class RKFResult
{
public:
  /// Time steps.
  std::vector<double> time;

  /// Solutions.
  std::vector<typename RKFTraits<ProblemType>::VariableType> y;

  /// Error estimate.
  double error_estimate = 0.0;

  /// Failure.
  bool failed = false;

  /// Number of time step expansions.
  unsigned int expansions = 0;

  /// Number of time step reductions.
  unsigned int reductions = 0;
};

/// Runge-Kutta-Fehlberg option class.
template <class ProblemType>
class RKFOptions
{
public:
  // Make the RKF class a friend for all its template combinations,
  // so they can access private members of this class.
  template <class, class>
  friend class RKF;

  using VariableType = typename RKFTraits<ProblemType>::VariableType;

  RKFOptions() = default;

  RKFOptions(const std::string &filename, const size_t &size)
  {
    parse_from_file(filename, size);
  }

private:
  void
  parse_from_file(const std::string &filename, const size_t &size)
  {
    GetPot datafile(filename.c_str());

    t0 = datafile("t0", 0.);
    tf = datafile("tf", 1.);

    if constexpr (std::is_same_v<VariableType, RKFType::Scalar>)
      {
        y0 = datafile("y0", 0.);
      }
    else // if constexpr (std::is_same_v<VariableType, RKFType::Scalar>)
      {
        // Read y0 as a list, but the size must be known.
        y0.resize(size);

        for (size_t i = 0; i < size; ++i)
          y0[i] = datafile("y0", static_cast<int>(i), 0.);
      }

    h0 = datafile("h0", 0.1);

    tolerance   = datafile("tolerance", 1e-6);
    n_max_steps = datafile("n_max_steps", 1e3);

    factor_reduction = datafile("factor_reduction", 0.95);
    factor_expansion = datafile("factor_expansion", 2);
  }

  /// Initial time.
  double t0;
  /// Final time.
  double tf;

  /// Initial condition.
  VariableType y0;

  /// Initial time step.
  double h0;

  /// Desired tolerance on error.
  double tolerance = 1e-6;
  /// Safeguard to avoid too many steps.
  unsigned int n_max_steps = 1e3;

  /// Multiplication factor for time step reduction.
  double factor_reduction = 0.95;
  /// Multiplication factor for time step expansion.
  double factor_expansion = 2;
};

/// Runge-Kutta-Fehlberg solver class.
template <class ButcherType, class ProblemType>
class RKF
{
public:
  using VariableType = typename RKFTraits<ProblemType>::VariableType;
  using Function     = typename RKFTraits<ProblemType>::ForcingTermType;

  /// Default constructor.
  RKF() = default;

  /// Constructor.
  RKF(const Function &               function_,
      const RKFOptions<ProblemType> &options_ = RKFOptions<ProblemType>())
    : options(options_)
    , table(ButcherType())
    , function(function_)
  {
    static_assert(is_any_v<ProblemType, RKFType::Scalar, RKFType::Vector>,
                  "Wrong problem type specified.");
  }

  /// Set options.
  void
  set_options(const RKFOptions<ProblemType> &options_)
  {
    options = options_;
  }

  /// Set the forcing term.
  void
  set_function(const Function &function_)
  {
    function = function_;
  }

  /// Set the Butcher array.
  void
  set_table(const ButcherType &table_)
  {
    table = table_;
  }

  RKFResult<ProblemType>
  operator()() const;

private:
  /**
   * Function for a single step. It is private since is used only internally.
   *
   * @param[in] t Time.
   * @param[in] y Solution at time t.
   * @param[in] h Time step.
   *
   * @return The low- and high-order solutions.
   */
  auto
  RKFstep(const double &t, const VariableType &y, const double &h) const
    -> std::pair<VariableType, VariableType>;

  RKFOptions<ProblemType> options;

  ButcherType table;
  Function    function;
};


template <class ButcherType, class ProblemType>
RKFResult<ProblemType>
RKF<ButcherType, ProblemType>::operator()() const
{
  const auto &[t0,
               tf,
               y0,
               h0,
               tolerance,
               n_max_steps,
               factor_reduction,
               factor_expansion] = options;

  RKFResult<ProblemType> result;

  auto &[time, y, error_estimate, failed, expansions, reductions] = result;

  error_estimate = 0.0;
  failed         = false;
  expansions     = 0;
  reductions     = 0;

  // Reserve some estimated space according to data.
  const size_t n_expected_steps =
    std::min(std::max(1u, 1 + static_cast<unsigned int>((tf - t0) / h0)),
             n_max_steps);

  time.reserve(n_expected_steps);
  y.reserve(n_expected_steps);

  // Push initial step.
  time.push_back(t0);
  y.push_back(y0);

  bool rejected = false;

  // Check that the time step does not becone ridiculously small.
  const double time_span = tf - t0;
  double       t         = t0;

  const double h_min = 100 * (tf - t0) * std::numeric_limits<double>::epsilon();
  double       h     = std::max(h0, h_min);

  VariableType y_curr = y0;

  unsigned int iter = 0;

  for (; iter < n_max_steps; ++iter)
    {
      // Low precision solution.
      VariableType y_low;

      // High precision solution.
      VariableType y_high;

      // Check if new time step will go past the final time.
      if (t + h < tf)
        {
          std::tie(y_low, y_high) = RKFstep(t, y_curr, h);
        }
      else
        {
          h = tf - t; // Truncate time step.

          if (h < h_min)
            {
              // Step ridiculously small. But we are at the end, so we may stop
              // here.
              y_low  = y_curr;
              y_high = y_curr;
            }
          else
            std::tie(y_low, y_high) = RKFstep(t, y_curr, h);
        }

      const double error = RKFTraits<ProblemType>::norm(y_low - y_high);

      if (error <= tolerance * h / time_span)
        {
          t = t + h;

          time.push_back(t);
          y.push_back(y_high);

          y_curr = y_high;

          error_estimate += error;

          if (!rejected && (t < tf))
            {
              h *= factor_expansion;
              ++expansions;
            }

          rejected = false;
        }
      else
        {
          rejected = true;
          ++reductions;

          h *= factor_reduction;
          h = std::max(h, h_min);
        }

      if (t >= tf)
        break;
    }

  if ((iter >= n_max_steps) && (t < tf))
    failed = true;

  return result;
}


template <class ButcherType, class ProblemType>
auto
RKF<ButcherType, ProblemType>::RKFstep(const double &      t,
                                       const VariableType &y,
                                       const double &      h) const
  -> std::pair<VariableType, VariableType>
{
  constexpr auto n_stages = ButcherType::n_stages();

  std::array<VariableType, n_stages> K;

  const auto &A  = table.A;
  const auto &b1 = table.b1;
  const auto &b2 = table.b2;
  const auto &c  = table.c;

  // The first step is always an Euler step.
  K[0] = function(t, y);
  for (unsigned int i = 1; i < n_stages; ++i)
    {
      const double time = t + c[i] * h;

      VariableType value = y;
      for (unsigned int j = 0; j < i; ++j)
        value += h * A[i][j] * K[j];

      K[i] = function(time, value);
    }

  VariableType v1 = y;
  VariableType v2 = y;
  for (unsigned int i = 0; i < n_stages; ++i)
    {
      v1 += h * K[i] * b1[i];
      v2 += h * K[i] * b2[i];
    }

  return std::make_pair(v1, v2);
}

/// Output stream for gnuplot.
/// Possible extension: export also an exact solution, if provided.
template <class ProblemType>
std::ostream &
operator<<(std::ostream &out, const RKFResult<ProblemType> &res)
{
  out << "# Number ot time steps: " << res.time.size() << std::endl
      << "# Number of reductions: " << res.reductions << std::endl
      << "# Number of expansions: " << res.expansions << std::endl
      << "# Error estimate: " << res.error_estimate << std::endl;

  double h_min = res.time[1] - res.time[0];
  double h_max = h_min;

  for (unsigned int i = 0; i < res.time.size() - 1; ++i)
    {
      const auto dt = res.time[i + 1] - res.time[i];
      h_max         = std::max(h_max, dt);
      h_min         = std::min(h_min, dt);
    }

  out << "# h_min: " << h_min << ", h_max: " << h_max << std::endl;

  out << "# t ";
  if constexpr (std::is_same_v<ProblemType, RKFType::Scalar>)
    out << "y";
  else // if constexpr (std::is_same_v<ProblemType, RKFType::Vector>)
    out << "y[0] ... y[" << res.y[0].size() - 1 << "]";

  out << std::endl;

  size_t i = 0;
  for (const auto &t : res.time)
    {
      out << "  " << t << "\t";
      const auto &yy = res.y[i];

      if constexpr (std::is_same_v<ProblemType, RKFType::Scalar>)
        {
          out << res.y[i];
        }
      else // if constexpr (std::is_same_v<ProblemType, RKFType::Vector>)
        {
          for (int k = 0; k < yy.size(); ++k)
            out << yy[k] << "\t";
        }

      out << "\n";
      ++i;
    }
  return out;
}

#endif /* RKF_HPP */
