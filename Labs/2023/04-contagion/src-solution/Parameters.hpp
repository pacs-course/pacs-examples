#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <string>

struct Domain
{
  // Domain is [0, domain_size] x [0, domain_size].
  double domain_size;

  // Pub size and location.
  double pub_size;

  double pub_x;
  double pub_y;
};

struct ContagionParameters
{
  // Number of agents.
  unsigned int n_agents;

  // Number of timesteps per day.
  unsigned int n_timesteps_per_day;

  // Simulation duration.
  unsigned int n_timesteps;

  // Number of agents initially infected.
  unsigned int n_init_infected;

  // Fraction of agents who practice social distancing.
  double frac_sd;

  // Agents get infected when another infected
  // gets closer than r_infection.
  double r_infection;
  // its square to avoid calculations
  double r_infection_2;
};

struct AgentParameters
{
  // Agents go to the pub once every n_timesteps_go_to_pub
  // timesteps.
  unsigned int n_timesteps_go_to_pub;

  // Number of timesteps spent at pub.
  unsigned int n_timesteps_at_pub;

  // Agents recover after n_timesteps_recover timesteps.
  unsigned int n_timesteps_recover;

  // Step length per timestep.
  double dr;
};

struct Parameters
{
  Parameters(const std::string &filename);

  Domain              domain;
  ContagionParameters contagion;
  AgentParameters     agent;
  int seed;
  int dump;
};

#endif /* PARAMETERS_HPP */
