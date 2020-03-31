#ifndef PERSON_HPP
#define PERSON_HPP

#include "Parameters.hpp"

#include <random>
#include <vector>

enum class Status
{
  Susceptible,
  Infected,
  Recovered
};

class Person
{
public:
  Person(const Status &initial_status = Status::Susceptible);

  void
  move();

  void
  update_contagion(std::vector<Person> &people);

  inline bool
  susceptible() const
  {
    return is_susceptible;
  }

  inline bool
  infected() const
  {
    return is_infected;
  }

  inline bool
  recovered() const
  {
    return is_recovered;
  }

protected:
  PersonParameters params;

  // Random numbers generators.
  std::default_random_engine                  engine;
  std::uniform_real_distribution<double>      rand;
  std::uniform_int_distribution<unsigned int> randi;

  // Position.
  double x;
  double y;
  double alpha;
  double dx;
  double dy;

  // Position backup.
  double x_bak;
  double y_bak;

  // Who does social distancing (sd)?
  bool does_sd;

  // Health status.
  bool is_susceptible;
  bool is_infected;
  bool is_recovered;

  double t_infected;

  // Market.
  int t_go_to_market; // Time until next shopping trip.

  unsigned int t_at_market; // Time spent at the market.
  bool         is_at_market;

private:
};

#endif /* PERSON_HPP */
