// Re-adapted from:
// https://github.com/seismotologist/coronaVirusContagion.

#ifndef CONTAGION_HPP
#define CONTAGION_HPP

#include "Parameters.hpp"
#include "Person.hpp"

#include <vector>

class Contagion
{
public:
  Contagion();

  void
  simulate();

protected:
  ContagionParameters params;

  std::vector<Person> people;

  std::vector<double>       day;
  std::vector<unsigned int> n_infected;
  std::vector<unsigned int> n_recovered;
  std::vector<unsigned int> n_susceptible;
};

#endif /* CONTAGION_HPP */
