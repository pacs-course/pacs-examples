#ifndef AGENT_HPP
#define AGENT_HPP

#include "Parameters.hpp"

#include <random>

enum class State {
  Susceptible,
  Infected,
  Recovered
};

class Agent
{
public:
  Agent(size_t seed, const State &initial_state, const Parameters &params);

  double x() const { return m_x; }
  double y() const { return m_y; }
  bool susceptible() const { return m_state == State::Susceptible; }
  bool infected() const { return m_state == State::Infected; }
  bool recovered() const { return m_state == State::Recovered; }
  State state() const { return m_state; }
  void set_state(State s) {m_state = s;}
  void move();

protected:
  std::pair<double, double> generate_direction_step();

  const Parameters m_params;
  double m_x, m_y; 
  State m_state;
  std::default_random_engine m_engine;
};

#endif /* AGENT_HPP */
