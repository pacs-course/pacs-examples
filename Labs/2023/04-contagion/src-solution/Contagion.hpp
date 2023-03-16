#ifndef CONTAGION_HPP
#define CONTAGION_HPP

#include "Agent.hpp"
#include "Parameters.hpp"

#include <string>
#include <vector>

class Contagion
{
public:
  Contagion(const std::string &filename);

  void
  run();

protected:
  void
  simulate();

  void
  output_results() const;

  const Parameters m_params;

  std::vector<Agent> m_agents;

  std::vector<double>       m_time;
  std::vector<unsigned int> m_n_susceptible;
  std::vector<unsigned int> m_n_infected;
  std::vector<unsigned int> m_n_recovered;
};

#endif /* CONTAGION_HPP */
