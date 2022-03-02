#include "Contagion.hpp"

Contagion::Contagion(const std::string &filename)
  : contagion_parameters(filename)
  , time(contagion_parameters.n_timesteps + 1)
  , n_susceptible(contagion_parameters.n_timesteps + 1)
  , n_infected(contagion_parameters.n_timesteps + 1)
  , n_recovered(contagion_parameters.n_timesteps + 1)
{
  people.reserve(contagion_parameters.n_people);

  
  for (size_t n = 0; n < contagion_parameters.n_people; ++n)
    {
      if (n < contagion_parameters.n_init_infected)
        people.emplace_back(filename, State::Infected);
      else
        people.emplace_back(filename, State::Susceptible);
    }
}

void
Contagion::run()
{
  for (unsigned int step = 0; step <= contagion_parameters.n_timesteps; ++step)
    {
      time[step] =
        static_cast<double>(step) / contagion_parameters.n_timesteps_per_day;

      for (size_t n = 0; n < people.size(); ++n)
        {
          // update position
          people[n].move();

          // update the infection state of people
          people[n].update_infection(people);

          if (people[n].is_susceptible())
            ++n_susceptible[n];
          else if (people[n].is_infected())
            ++n_infected[n];
          else // if (people[n].is_recovered())
            ++n_recovered[n];
        }
    }

  
}
