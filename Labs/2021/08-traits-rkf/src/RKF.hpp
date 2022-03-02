#ifndef RKF_HPP
#define RKF_HPP


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
