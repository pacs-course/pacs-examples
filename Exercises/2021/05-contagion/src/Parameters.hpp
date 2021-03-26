#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <string>

class Domain
{
public:
  Domain(const std::string &filename);

  double domain_size;

  double pub_size;
  double pub_x;
  double pub_y;
};

class ContagionParameters
{
public:
  ContagionParameters(const std::string &filename);

  unsigned int n_people;
  unsigned int n_timesteps_per_day;
  unsigned int n_timesteps;

  unsigned int n_init_infected;

  double frac_sd;
  double r_infection;
};

class PersonParameters
{
public:
  PersonParameters(const std::string &filename);

  unsigned int n_timesteps_go_to_pub;
  unsigned int n_timesteps_at_pub;
  unsigned int n_timesteps_recover;

  double dr;
};


#endif /* PARAMETERS_HPP */
