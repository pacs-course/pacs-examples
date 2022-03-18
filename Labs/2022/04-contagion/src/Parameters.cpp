#include "GetPot"
#include "Parameters.hpp"

Parameters::Parameters(const std::string &filename)
{
  GetPot file(filename.c_str());

  // Domain.
  {
    const std::string subsection = "domain/";

    domain.domain_size = file((subsection + "domain_size").c_str(), 1.0);

    domain.pub_size = file((subsection + "pub_size").c_str(), 0.1);

    domain.pub_x = file((subsection + "pub_x").c_str(), 0.5);
    domain.pub_y = file((subsection + "pub_y").c_str(), 0.5);
  }

  // Contagion.
  {
    const std::string subsection = "contagion/";

    contagion.n_agents = file((subsection + "n_agents").c_str(), 200);

    contagion.n_timesteps_per_day =
      file((subsection + "n_timesteps_per_day").c_str(), 24);

    contagion.n_timesteps =
      file((subsection + "n_timesteps").c_str(),
           60 * static_cast<int>(contagion.n_timesteps_per_day));

    contagion.n_init_infected =
      file((subsection + "n_init_infected").c_str(), 10);

    contagion.frac_sd = file((subsection + "frac_sd").c_str(), 0.75);

    contagion.r_infection = file((subsection + "r_infection").c_str(), 0.04);
  }

  // Agent.
  {
    const std::string subsection = "agent/";

    agent.n_timesteps_go_to_pub =
      file((subsection + "n_timesteps_go_to_pub").c_str(),
           7 * static_cast<int>(contagion.n_timesteps_per_day));

    agent.n_timesteps_at_pub =
      file((subsection + "n_timesteps_at_pub").c_str(), 3);

    agent.n_timesteps_recover =
      file((subsection + "n_timesteps_recover").c_str(),
           10 * static_cast<int>(contagion.n_timesteps_per_day));

    agent.dr = file((subsection + "dr").c_str(), 0.01);
  }
}
