#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

class ContagionParameters
{
public:
  ContagionParameters() = default;

  // Number of people.
  unsigned int n_people = 200;

  // Number of timesteps per day.
  unsigned int n_timesteps_per_day = 5;

  // Simulation duration.
  unsigned int n_timesteps = 150;

  // Initial fraction of infected people.
  double frac_initial_infected = 0.01;

  // Fraction of people who practice social distancing.
  double frac_sd = 0.75;

  // Market size and location.
  double market_size = 0.1;

  double market_x = 0.5;
  double market_y = 0.5;
};

class PersonParameters : public ContagionParameters
{
public:
  PersonParameters() = default;

  // People go to the market once every n_timesteps_go_to_market
  // timesteps.
  unsigned int n_timesteps_go_to_market = 5 * n_timesteps_per_day;

  // Number of time steps spent at market.
  unsigned int n_timesteps_at_market = 1;

  // People recover after n_timesteps_recover timesteps.
  unsigned int n_timesteps_recover = 10 * n_timesteps_per_day;

  // People get infected when an already infected is closer than
  // r_infection.
  double r_infection = 0.04;

  // Step length per time step.
  double dr = 0.02;
};

#endif /* PARAMETERS_HPP */
