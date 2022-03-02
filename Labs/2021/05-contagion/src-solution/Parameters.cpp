#include "GetPot"
#include "Parameters.hpp"

Domain::Domain(const std::string &filename)
{
  GetPot file(filename.c_str());

  const std::string subsection = "domain/";

  domain_size = file((subsection + "domain_size").c_str(), 1.0);

  pub_size = file((subsection + "pub_size").c_str(), 0.1);

  pub_x = file((subsection + "pub_x").c_str(), 0.5);
  pub_y = file((subsection + "pub_y").c_str(), 0.5);
}

ContagionParameters::ContagionParameters(const std::string &filename)
  : Domain(filename)
{
  GetPot file(filename.c_str());

  const std::string subsection = "contagion/";

  n_people = file((subsection + "n_people").c_str(), 200);

  n_timesteps_per_day = file((subsection + "n_timesteps_per_day").c_str(), 24);

  n_timesteps =
    file((subsection + "n_timesteps").c_str(), 60 * int(n_timesteps_per_day));

  n_init_infected = file((subsection + "n_init_infected").c_str(), 10);

  frac_sd = file((subsection + "frac_sd").c_str(), 0.75);

  r_infection = file((subsection + "r_infection").c_str(), 0.04);
}

PersonParameters::PersonParameters(
  const std::string &                               filename,
  const std::shared_ptr<const ContagionParameters> &params_contagion_)
  : params_contagion(params_contagion_)
{
  GetPot file(filename.c_str());

  const std::string subsection = "person/";

  n_timesteps_go_to_pub = file((subsection + "n_timesteps_go_to_pub").c_str(),
                               7 * int(params_contagion->n_timesteps_per_day));

  n_timesteps_at_pub = file((subsection + "n_timesteps_at_pub").c_str(), 3);

  n_timesteps_recover = file((subsection + "n_timesteps_recover").c_str(),
                             10 * int(params_contagion->n_timesteps_per_day));

  dr = file((subsection + "dr").c_str(), 0.01);
}
