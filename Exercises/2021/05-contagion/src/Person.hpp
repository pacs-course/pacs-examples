#ifndef PERSON_HPP
#define PERSON_HPP

#include "Parameters.hpp"

#include <random>
#include <string>

enum class State
{
  Susceptible,
  Infected,
  Recovered
};

enum class Move
{
  Stay,
  Walk,
  Go_To_Pub,
  Return_From_Pub
};

class Person
{
public:
  Person(const std::string &filename, const State &initial_state);

  void
  move();

  void
  update_infection(std::vector<Person> &people);

  bool
  is_susceptible() const
  {
    return (state == State::Susceptile);
  }

  bool
  is_infected() const
  {
    return (state == State::Infected);
  }

  bool
  is_recovered() const
  {
    return (state == State::Recovered);
  }
  
private:
  Domain              domain_params;
  ContagionParameters contagion_params;
  PersonParameters    params;

  std::default_random_engine                  engine;
  std::uniform_real_distribution<double>      rand;
  std::uniform_int_distribution<unsigned int> randi;

  State state;

  double x;
  double y;

  double x_old;
  double y_old;

  bool         does_sd;
  unsigned int t_infection;

  bool         is_at_pub;
  int          t_go_to_pub;
  unsigned int t_spent_at_pub;
};

#endif
