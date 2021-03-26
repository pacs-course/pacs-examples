#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <memory>
#include <string>

class Domain
{
public:
  Domain(const std::string &filename);

  // Domain is [0, domain_size] x [0, domain_size].
  double domain_size;

  // Pub size and location.
  double pub_size;

  double pub_x;
  double pub_y;
};

class ContagionParameters : public Domain
{
public:
  ContagionParameters(const std::string &filename);

  // Number of people.
  unsigned int n_people;

  // Number of timesteps per day.
  unsigned int n_timesteps_per_day;

  // Simulation duration.
  unsigned int n_timesteps;

  // Number of people initially infected.
  unsigned int n_init_infected;

  // Fraction of people who practice social distancing.
  double frac_sd;

  // People get infected when another infected
  // gets closer than r_infection.
  double r_infection;
};

class PersonParameters
{
public:
  PersonParameters(
    const std::string &                               filename,
    const std::shared_ptr<const ContagionParameters> &params_contagion_);

  // Contagion parameters.
  std::shared_ptr<const ContagionParameters> params_contagion;

  // People go to the pub once every n_timesteps_go_to_pub
  // timesteps.
  unsigned int n_timesteps_go_to_pub;

  // Number of timesteps spent at pub.
  unsigned int n_timesteps_at_pub;

  // People recover after n_timesteps_recover timesteps.
  unsigned int n_timesteps_recover;

  // Step length per timestep.
  double dr;
};

#endif /* PARAMETERS_HPP */
