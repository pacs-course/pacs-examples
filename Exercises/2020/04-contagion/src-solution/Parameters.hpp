#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <memory>
#include <string>

class ContagionParameters
{
public:
  ContagionParameters(const std::string &filename);

  // Number of people.
  unsigned int n_people;

  // Domain is [0, domain_size] x [0, domain_size].
  double domain_size;

  // Number of timesteps per day.
  unsigned int n_timesteps_per_day;

  // Simulation duration.
  unsigned int n_timesteps;

  // Fraction of people who practice social distancing.
  double frac_sd;

  // People get infected when another infected
  // gets closer than r_infection.
  double r_infection;

  // Market size and location.
  double market_size;

  double market_x;
  double market_y;
};

class PersonParameters
{
public:
  PersonParameters(const std::string &filename,
                   const std::shared_ptr<const ContagionParameters>
                     &params_contagion_);

  std::shared_ptr<const ContagionParameters> params_contagion;

  // People go to the market once every n_timesteps_go_to_market
  // timesteps.
  unsigned int n_timesteps_go_to_market =
    5 * params_contagion->n_timesteps_per_day;

  // Number of timesteps spent at market.
  unsigned int n_timesteps_at_market = 2;

  // People recover after n_timesteps_recover timesteps.
  unsigned int n_timesteps_recover =
    10 * params_contagion->n_timesteps_per_day;

  // Step length per timestep.
  double dr = 0.01;
};

#endif /* PARAMETERS_HPP */
