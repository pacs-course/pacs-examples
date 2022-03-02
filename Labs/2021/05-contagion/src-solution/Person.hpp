#ifndef PERSON_HPP
#define PERSON_HPP

#include "Parameters.hpp"

#include <memory>
#include <random>
#include <utility>
#include <vector>

enum class State
{
  Susceptible,
  Infected,
  Recovered
};

enum class Move
{
  Walk,
  Stay,
  Go_To_Pub,
  Return_From_Pub
};

class Person
{
public:
  Person(const State &initial_state, const PersonParameters &params);

  void
  move();

  void
  update_contagion(std::vector<Person> &people);

  inline bool
  susceptible() const
  {
    return (state == State::Susceptible);
  }

  inline bool
  infected() const
  {
    return (state == State::Infected);
  }

  inline bool
  recovered() const
  {
    return (state == State::Recovered);
  }

protected:
  // Generate new random direction step.
  std::pair<double, double>
  generate_direction_step();

  const PersonParameters params;

  // Random numbers generators.
  std::default_random_engine                  engine;
  std::uniform_real_distribution<double>      rand;
  std::uniform_int_distribution<unsigned int> randi;

  // Position and backup.
  double x;
  double y;
  double x_bak;
  double y_bak;

  // Does social distancing?
  bool does_sd;

  // Health status.
  State state;

  // Infection duration.
  unsigned int t_infection;

  // Pub time?
  bool is_at_pub;

  // Timesteps until next pub trip.
  int t_go_to_pub;

  // Timesteps spent at the pub.
  unsigned int t_spent_at_pub;
};

#endif /* PERSON_HPP */
