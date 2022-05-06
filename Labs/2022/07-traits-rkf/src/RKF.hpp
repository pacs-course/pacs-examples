#ifndef RKF_HPP
#define RKF_HPP

#include "ButcherRKF.hpp"
#include "RKFTraits.hpp"

#include <iostream>
#include <utility>
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

/// Runge-Kutta-Fehlberg solver class.
template <class ButcherType, class ProblemType>
class RKF
{
public:
  using VariableType = typename RKFTraits<ProblemType>::VariableType;
  using Function     = typename RKFTraits<ProblemType>::ForcingTermType;

  RKF(const ButcherType &table_, const Function &function_)
    : table(table_)
    , function(function_)
  {}

  RKFResult<ProblemType>
  operator()(const double &      t0,
             const double &      tf,
             const VariableType &y0,
             const double &      h0,
             const double &      tol,
             const unsigned int  n_max_steps,
             const double        factor_reduction = 0.95,
             const double        factor_expansion = 2) const;

private:
  auto
  RKFstep(const double &t, const VariableType &y, const double &h) const
    -> std::pair<VariableType, VariableType>;

  ButcherType table;
  Function    function;
};


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

      out << std::endl;
      ++i;
    }
  return out;
}

#endif /* RKF_HPP */
