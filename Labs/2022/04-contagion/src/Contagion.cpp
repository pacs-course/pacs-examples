#include "Contagion.hpp"
#include "gnuplot-iostream.hpp"

Contagion::Contagion(const std::string &filename)
  : params_contagion(std::make_shared<const ContagionParameters>(filename))
{}

void
Contagion::run()
{}

void
Contagion::output_results() const
{
  // Output results to CSV file.

  // Plot results.
  Gnuplot gp;
  gp << "set term png;" << std::endl
     << "set output \"output.png\";" << std::endl
     << "set xlabel 'Time [days]';" << std::endl
     << "set ylabel 'No. of agents';" << std::endl
     << "set key center right;" << std::endl
     << "plot " << gp.file1d(std::tie(time, n_susceptible))
     << "with line linewidth 2 title 'Susceptible',"
     << gp.file1d(std::tie(time, n_infected))
     << "with line linewidth 2 title 'Infected',"
     << gp.file1d(std::tie(time, n_recovered))
     << "with line linewidth 2 title 'Recovered'" << std::endl;
}
