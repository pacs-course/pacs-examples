#include "Contagion.hpp"

#include "gnuplot-iostream.hpp"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>

Contagion::Contagion()
  : people(params.n_people - 1)
  , day(params.n_timesteps)
  , n_infected(params.n_timesteps)
  , n_recovered(params.n_timesteps)
  , n_susceptible(params.n_timesteps)
{
  people.emplace_back(Person(Status::Infected));
}

void
Contagion::simulate()
{
  std::cout << "Running simulation..." << std::endl;

  for (unsigned int step = 0; step < params.n_timesteps; ++step)
    {
      day[step] =
        static_cast<double>(step + 1) / params.n_timesteps_per_day;

      for (auto &p : people)
        {
          p.move();
          p.update_contagion(people);
        }

      // Update people count.
      for (const auto &p : people)
        {
          if (p.infected())
            ++n_infected[step];
          else if (p.recovered())
            ++n_recovered[step];
          else
            ++n_susceptible[step];
        }
    }

  // Export output.
  std::ofstream file("output.csv");
  file << "day, n_infected, n_recovered, n_susceptible" << std::endl;

  for (unsigned int step = 0; step < params.n_timesteps; ++step)
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
}
