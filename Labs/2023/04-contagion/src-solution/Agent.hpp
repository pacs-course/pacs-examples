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
  Agent(size_t seed, const State &initial_state, const Parameters &params);

  void
  move();

  inline State 
  state() const {
    return m_state;
  }

  inline bool
  susceptible() const
  {
    return (m_state == State::Susceptible);
  }

  inline bool
  infected() const
  {
    return (m_state == State::Infected);
  }

  inline bool
  recovered() const
  {
    return (m_state == State::Recovered);
  }

  inline bool
  is_home() const {
    return (m_x == m_x_bak) && (m_y == m_y_bak);
  }

  inline double
  x() const {
    return m_x;
  }

  inline double
  y() const {
    return m_y;
  }

  void set_state(State s) {
    m_state = s;
  }

protected:
  std::pair<double, double> generate_direction_step();

  // its a bit rendundant to keep a copy of parameters in each agent,
  // we could use:
  // * a reference if we are sure it exists somewhere else
  //    e.g. in the Contagion instance, in this case we are using composition
  // * a shared pointer, this is more general, Agent could exist without a Contagion
  //    instance, we are using aggregation
  const Parameters &m_params;

  // Engine for random numbers generators.
  std::default_random_engine m_engine;

  // Position and backup.
  double m_x;
  double m_y;
  double m_x_bak;
  double m_y_bak;

  // Does social distancing?
  bool m_does_sd;

  // Health status.
  State m_state;

  // Infection duration.
  unsigned int m_t_infection;

  // Pub time?
  bool m_is_at_pub;

  // Timesteps until next pub trip.
  std::optional<unsigned int> m_t_go_to_pub;

  // Timesteps spent at the pub.
  unsigned int m_t_spent_at_pub;
};

#endif /* AGENT_HPP */
