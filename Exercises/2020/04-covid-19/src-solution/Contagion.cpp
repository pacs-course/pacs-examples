#include "Contagion.hpp"

#include "gnuplot-iostream.hpp"

#include <algorithm>
#include <execution>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>

Contagion::Contagion(
  const std::shared_ptr<const ContagionParameters> &params_contagion_)
  : params_contagion(params_contagion_)
  , day(params_contagion->n_timesteps)
  , n_infected(params_contagion->n_timesteps)
  , n_recovered(params_contagion->n_timesteps)
  , n_susceptible(params_contagion->n_timesteps)
{
  people.reserve(params_contagion->n_people);

  for (size_t p = 0; p < people.capacity() - 1; ++p)
    {
      people.emplace_back(params_contagion);
    }

  people.emplace_back(params_contagion, State::Infected);
}

void
Contagion::simulate()
{
  std::cout << "Running... ";

  for (unsigned int step = 0; step < params_contagion->n_timesteps;
       ++step)
    {
      day[step] = static_cast<double>(step + 1) /
                  params_contagion->n_timesteps_per_day;

      std::for_each(people.begin(), people.end(), [this](Person &p) {
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

  // Export output.
  std::ofstream file("output.csv");
  file << "day, n_infected, n_recovered, n_susceptible" << std::endl;

  for (unsigned int step = 0; step < params_contagion->n_timesteps;
       ++step)
    {
      file << day[step] << ", " << n_infected[step] << ", "
           << n_recovered[step] << ", " << n_susceptible[step]
           << std::endl;
    }
  file.close();

  Gnuplot gp;
  gp << "set xlabel 'Day'; set ylabel 'No. of people'; plot"
     << gp.file1d(std::tie(day, n_infected))
     << "with line linewidth 2 title 'Infected',"
     << gp.file1d(std::tie(day, n_recovered))
     << "with line linewidth 2 title 'Recovered',"
     << gp.file1d(std::tie(day, n_susceptible))
     << "with line linewidth 2 title 'Susceptible'" << std::endl;

  std::cout << "Done!" << std::endl;
}
