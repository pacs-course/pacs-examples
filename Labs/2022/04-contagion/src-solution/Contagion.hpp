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

  const Parameters params;

  std::vector<Agent> agents;

  std::vector<double>       time;
  std::vector<unsigned int> n_susceptible;
  std::vector<unsigned int> n_infected;
  std::vector<unsigned int> n_recovered;
};

#endif /* CONTAGION_HPP */
