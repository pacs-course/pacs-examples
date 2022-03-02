#ifndef CONTAGION_HPP
#define CONTAGION_HPP

#include "Parameters.hpp"
#include "Person.hpp"

#include <memory>
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

  std::shared_ptr<const ContagionParameters> params_contagion;

  std::vector<Person> people;

  std::vector<double>       time;
  std::vector<unsigned int> n_susceptible;
  std::vector<unsigned int> n_infected;
  std::vector<unsigned int> n_recovered;
};

#endif /* CONTAGION_HPP */
