#include "GetPot"
#include "helperfunction.hpp"
#include <cmath>
#include <cstddef>
#include <format>
#include <iomanip>
namespace apsc::NumericalIntegration
{
void
readParameters(const int argc, char **argv, double &a, double &b,
               unsigned int &nint, double &targetError, std::ostream &out)
{
  GetPot       cl(argc, argv);
  const double pi = 2 * std::atan(1.0);
  if(cl.search(2, "--help", "-h"))
    {
      out << "Compute integral from a to b with nint intervals" << "\n";
      out << "Possible arguments:" << "\n";
      out << "a=Value  (default 0)" << "\n";
      out << "b=Value (default pi)" << "\n";
      out << "nint=Value (default 10)" << "\n";
      out << "targetError=Value (default 1e-4)" << "\n";
      out << "-h or --help : this help" << "\n";
      std::exit(1);
    }
  a = cl("a", 0.);
  b = cl("b", 1 * pi);
  nint = cl("nint", 10);
  targetError = cl("targetError", 1e-4);
}

void
readParameters(const int argc, char **argv, double &a, double &b,
               unsigned int &nint, double &targetError, std::string &library,
               std::string &integrand, std::ostream &out)
{
  GetPot       cl(argc, argv);
  const double pi = 2 * std::atan(1.0);
  if(cl.search(2, "--help", "-h"))
    {
      out << "Compute integral from a to b with nint intervals" << "\n";
      out << "Possible arguments:" << "\n";
      out << "a=Value  (default 0)" << "\n";
      out << "b=Value (default pi)" << "\n";
      out << "nint=Value (default 10)" << "\n";
      out << "targetError=Value (default 1e-4)" << "\n";
      out << "library=string (default libintegrands.so)" << "\n";
      out << "integrand=string (default fsincos)" << "\n";
      std::exit(1);
    }
  a = cl("a", 0.);
  b = cl("b", 1 * pi);
  nint = cl("nint", 10);
  targetError = cl("targetError", 1e-4);
  library = cl("library", "./libintegrands.so");
  integrand = cl("integrand", "fsincos");
}

void
printout(double const value, double const exact, double const error,
         std::string const &title, std::ostream &out)
{
  out << std::format("*** {} ***\n", title);
  out << std::format("Computed value {:<20.15}\n", value);
  out << std::format("Error          {:<20.15}\n", exact - value);
  out << std::format("Target error   {:<20.15}\n\n", error);
}

} // namespace apsc::NumericalIntegration
