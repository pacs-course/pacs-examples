#ifndef CONTAGION_HPP
#define CONTAGION_HPP

#include "Parameters.hpp"
#include "Person.hpp"

#include <memory>
#include <vector>

class Contagion
{
public:
  Contagion(const std::shared_ptr<const ContagionParameters>
              &params_contagion_);

  void
  simulate();

protected:
  std::shared_ptr<const ContagionParameters> params_contagion;

  std::vector<Person> people;

  std::vector<double>       day;
  std::vector<unsigned int> n_infected;
  std::vector<unsigned int> n_recovered;
  std::vector<unsigned int> n_susceptible;
};

#endif /* CONTAGION_HPP */
