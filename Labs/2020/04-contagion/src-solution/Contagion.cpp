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
  : params_contagion(
      std::make_shared<const ContagionParameters>(filename))
  , day(params_contagion->n_timesteps + 1)
  , n_susceptible(params_contagion->n_timesteps + 1)
  , n_infected(params_contagion->n_timesteps + 1)
  , n_recovered(params_contagion->n_timesteps + 1)
{
  people.reserve(params_contagion->n_people);

  PersonParameters params(filename, params_contagion);

  for (size_t p = 0; p < people.capacity() - 1; ++p)
    {
      people.emplace_back(State::Susceptible, params);
    }

  people.emplace_back(State::Infected, params);
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
  day[0] = 0;

  n_susceptible[0] = params_contagion->n_people - 1;
  n_infected[0]    = 1;
  n_recovered[0]   = 0;

  for (unsigned int step = 1; step <= params_contagion->n_timesteps;
       ++step)
    {
      std::cout << "Timestep " << step << " / "
                << params_contagion->n_timesteps << "\r";

      day[step] = static_cast<double>(step) /
                  params_contagion->n_timesteps_per_day;

      // This loop can not be parallelized.
      std::for_each(std::execution::seq,
                    people.begin(),
                    people.end(),
                    [this](Person &p) {
                      p.move();
                      p.update_contagion(people);
                    });

      // Update people count.
      n_susceptible[step] = std::count_if(std::execution::par_unseq,
                                          people.begin(),
                                          people.end(),
                                          [](const Person &p) {
                                            return p.susceptible();
                                          });

      n_infected[step] =
        std::count_if(std::execution::par_unseq,
                      people.begin(),
                      people.end(),
                      [](const Person &p) { return p.infected(); });

      n_recovered[step] =
        std::count_if(std::execution::par_unseq,
                      people.begin(),
                      people.end(),
                      [](const Person &p) { return p.recovered(); });
    }

  std::cout << std::endl;

  // Find peak.
  auto max_n_infected =
    std::max_element(n_infected.begin(), n_infected.end());
  auto timestep_peak =
    std::distance(n_infected.begin(), max_n_infected);

  std::cout << "Peak of " << *max_n_infected << " infected ("
            << static_cast<double>(*max_n_infected) /
                 params_contagion->n_people * 100
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
  file << "day, n_susceptible, n_infected, n_recovered" << std::endl;

  for (unsigned int step = 0; step <= params_contagion->n_timesteps;
       ++step)
    {
      file << day[step] << ", " << n_susceptible[step] << ", "
           << n_infected[step] << ", " << n_recovered[step]
           << std::endl;
    }
  file.close();

  // Plot results.
  Gnuplot gp;
  gp
    << "set xlabel 'Day'; set ylabel 'No. of people'; set key center "
       "right; plot "
    << gp.file1d(std::tie(day, n_susceptible))
    << "with line linewidth 2 title 'Susceptible',"
    << gp.file1d(std::tie(day, n_infected))
    << "with line linewidth 2 title 'Infected',"
    << gp.file1d(std::tie(day, n_recovered))
    << "with line linewidth 2 title 'Recovered'" << std::endl;
}
