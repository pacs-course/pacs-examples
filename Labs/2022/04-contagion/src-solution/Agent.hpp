#ifndef AGENT_HPP
#define AGENT_HPP

#include "Parameters.hpp"

#include <optional>
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

class Agent
{
public:
  Agent(const State &initial_state, const Parameters &params);

  void
  move();

  void
  update_contagion(std::vector<Agent> &agents);

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
  const Parameters params;

  // Engine for random numbers generators.
  std::default_random_engine engine;

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
  std::optional<unsigned int> t_go_to_pub;

  // Timesteps spent at the pub.
  unsigned int t_spent_at_pub;
};

#endif /* AGENT_HPP */
