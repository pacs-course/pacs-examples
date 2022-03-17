#include "Contagion.hpp"
#include "gnuplot-iostream.hpp"

#include <algorithm>
#include <cmath>
#include <execution>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>

Contagion::Contagion(const std::string &filename)
  : params_contagion(std::make_shared<const ContagionParameters>(filename))
  , time(params_contagion->n_timesteps + 1)
  , n_susceptible(params_contagion->n_timesteps + 1)
  , n_infected(params_contagion->n_timesteps + 1)
  , n_recovered(params_contagion->n_timesteps + 1)
{
  agents.reserve(params_contagion->n_agents);

  AgentParameters params(filename, params_contagion);

  for (size_t p = 0; p < agents.capacity() - params_contagion->n_init_infected;
       ++p)
    {
      agents.emplace_back(State::Susceptible, params);
    }

  for (size_t p = 0; p < params_contagion->n_init_infected; ++p)
    agents.emplace_back(State::Infected, params);
}

void
Contagion::run()
{
  simulate();
  output_results();
}

void
Contagion::simulate()
{
  for (unsigned int step = 0; step <= params_contagion->n_timesteps; ++step)
    {
      std::cout << "Timestep " << step << " / " << params_contagion->n_timesteps
                << "\r";

      time[step] =
        static_cast<double>(step) / params_contagion->n_timesteps_per_day;

      if (step >= 1)
        {
          // This loop can not be parallelized.
          std::for_each(std::execution::seq,
                        agents.begin(),
                        agents.end(),
                        [this](Agent &p) {
                          p.move();
                          p.update_contagion(agents);
                        });
        }

      // Update agents count.
      n_susceptible[step] =
        std::count_if(std::execution::par_unseq,
                      agents.begin(),
                      agents.end(),
                      [](const Agent &p) { return p.susceptible(); });

      n_infected[step] =
        std::count_if(std::execution::par_unseq,
                      agents.begin(),
                      agents.end(),
                      [](const Agent &p) { return p.infected(); });

      n_recovered[step] =
        std::count_if(std::execution::par_unseq,
                      agents.begin(),
                      agents.end(),
                      [](const Agent &p) { return p.recovered(); });
    }

  std::cout << std::endl;

  // Find peak.
  const auto max_n_infected =
    std::max_element(n_infected.begin(), n_infected.end());

  const auto timestep_peak = std::distance(n_infected.begin(), max_n_infected);

  std::cout << "Peak of " << *max_n_infected << " infected ("
            << static_cast<double>(*max_n_infected) /
                 params_contagion->n_agents * 100
            << "%) reached after "
            << std::ceil(static_cast<double>(timestep_peak) /
                         params_contagion->n_timesteps_per_day)
            << " days." << std::endl;
}

void
Contagion::output_results() const
{
  // Output results to CSV file.
  std::ofstream file("output.csv", std::ofstream::out);
  file << "time, n_susceptible, n_infected, n_recovered" << std::endl;

  for (unsigned int step = 0; step <= params_contagion->n_timesteps; ++step)
    {
      file << time[step] << ", " << n_susceptible[step] << ", "
           << n_infected[step] << ", " << n_recovered[step] << std::endl;
    }
  file.close();

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
