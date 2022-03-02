#include "Parameters.hpp"

#include "GetPot"

ContagionParameters::ContagionParameters(const std::string &filename)
{
  GetPot file(filename.data());

  std::string subsection = "contagion/";

  n_people = file((subsection + "n_people").data(), 200);

  domain_size = file((subsection + "domain_size").data(), 1.0);

  n_timesteps_per_day =
    file((subsection + "n_timesteps_per_day").data(), 24);

  n_timesteps = file((subsection + "n_timesteps").data(),
                     60 * int(n_timesteps_per_day));

  frac_sd = file((subsection + "frac_sd").data(), 0.75);

  r_infection = file((subsection + "r_infection").data(), 0.04);

  market_size = file((subsection + "market_size").data(), 0.1);

  market_x = file((subsection + "market_x").data(), 0.5);
  market_y = file((subsection + "market_y").data(), 0.5);
}

PersonParameters::PersonParameters(
  const std::string &                               filename,
  const std::shared_ptr<const ContagionParameters> &params_contagion_)
  : params_contagion(params_contagion_)
{
  GetPot file(filename.data());

  std::string subsection = "person/";

  n_timesteps_go_to_market =
    file((subsection + "n_timesteps_go_to_market").data(),
         5 * int(params_contagion->n_timesteps_per_day));

  n_timesteps_at_market =
    file((subsection + "n_timesteps_at_market").data(), 2);

  n_timesteps_recover =
    file((subsection + "n_timesteps_recover").data(),
         10 * int(params_contagion->n_timesteps_per_day));

  dr = file((subsection + "dr").data(), 0.01);
}
