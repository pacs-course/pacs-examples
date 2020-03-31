// Re-adapted from:
// https://github.com/seismotologist/coronaVirusContagion.

#ifndef CONTAGION_HPP
#define CONTAGION_HPP

#include <random>
#include <vector>

class ContagionParameters
{
public:
  ContagionParameters() = default;

  bool verbose = false;

  // Number of people.
  unsigned int n_people = 200;

  // Number of timesteps per day.
  unsigned int n_timesteps_per_day = 5;

  // Simulation duration.
  unsigned int n_timesteps = 150;

  // Fraction of people who practice social distancing.
  double sd_frac = 0.75;

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

  // Market size and location.
  double market_size = 0.1;

  double market_x = 0.5;
  double market_y = 0.5;
};

class Contagion
{
public:
  Contagion(const ContagionParameters &params_);

  void
  simulate();

private:
  ContagionParameters params;

  // Random numbers generators.
  std::default_random_engine                  engine;
  std::uniform_real_distribution<double>      rand;
  std::uniform_int_distribution<unsigned int> randi;

  // Position.
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> alpha;
  std::vector<double> dx;
  std::vector<double> dy;

  // Position backup.
  std::vector<double> x_bak;
  std::vector<double> y_bak;

  // Who does social distancing (sd)?
  std::vector<bool> does_sd;

  // Health status.
  std::vector<bool> is_susceptible;
  std::vector<bool> is_infected;
  std::vector<bool> is_recovered;

  std::vector<double> t_infected;

  // Market.
  std::vector<int> t_go_to_market; // Time until next shopping trip.

  std::vector<unsigned int> t_at_market; // Time spent at the market.
  std::vector<bool>         is_at_market;
};

#endif /* CONTAGION_HPP */
