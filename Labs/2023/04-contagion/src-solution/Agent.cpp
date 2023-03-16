#include "Agent.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <execution>
#include <limits>
#include <iostream>

// Since C++20.
#include <numbers>

Agent::Agent(size_t seed, const State &initial_state, const Parameters &params)
  : m_params(params)
  , m_engine(seed)
  , m_does_sd(false)
  , m_state(initial_state)
  , m_t_infection(0)
  , m_is_at_pub(false)
  , m_t_spent_at_pub(0)
{
  std::uniform_real_distribution<double> rand(0, 1);

  m_x = rand(m_engine) * params.domain.domain_size;
  m_y = rand(m_engine) * params.domain.domain_size;

  m_does_sd = (rand(m_engine) < params.contagion.frac_sd);

  std::uniform_int_distribution<unsigned int> randi(
    1, params.agent.n_timesteps_go_to_pub);
  m_t_go_to_pub = randi(m_engine);
}

void
Agent::move()
{
  if (m_t_go_to_pub.has_value())
    --(m_t_go_to_pub.value());

  if (m_is_at_pub)
    {
      ++m_t_spent_at_pub;
    }
  else
    {
      m_x_bak = m_x;
      m_y_bak = m_y;
    }

  // Determine next move.
  Move next_move;
  if (m_t_go_to_pub > 0)
    {
      if (m_does_sd)
        {
          next_move = Move::Stay;
        }
      else
        {
          next_move = Move::Walk;
        }
    }
  else if (m_is_at_pub)
    {
      if (m_t_spent_at_pub < m_params.agent.n_timesteps_at_pub)
        {
          next_move = Move::Stay;
        }
      else
        {
          next_move = Move::Return_From_Pub;
        }
    }
  else // if (t_go_to_pub == 0)
    {
      next_move = Move::Go_To_Pub;
    }

  if (next_move == Move::Walk)
    {
      // try to move until we do not hit a wall or exceed maximum
      // tries and we stay still
      for(int i = 0; i < 1000; ++i) {
        const auto [dx, dy] = generate_direction_step();
        const auto x_new = m_x + dx;
        const auto y_new = m_y + dy;
        const auto inside = (x_new >= 0) && (x_new <= m_params.domain.domain_size) 
                         && (y_new >= 0) && (y_new <= m_params.domain.domain_size);
        if(inside) {
          m_x = x_new;
          m_y = y_new;
          break;
        }
      }
      
    }
  else if (next_move == Move::Go_To_Pub)
    {
      std::uniform_real_distribution<double> rand_x(
        m_params.domain.pub_x - 0.5 * m_params.domain.pub_size,
        m_params.domain.pub_x + 0.5 * m_params.domain.pub_size);

      std::uniform_real_distribution<double> rand_y(
        m_params.domain.pub_y - 0.5 * m_params.domain.pub_size,
        m_params.domain.pub_y + 0.5 * m_params.domain.pub_size);

      m_x = rand_x(m_engine);
      m_y = rand_y(m_engine);

      m_t_go_to_pub = {};
      m_is_at_pub   = true;
    }
  else if (next_move == Move::Return_From_Pub)
    {
      m_x = m_x_bak;
      m_y = m_y_bak;

      m_t_go_to_pub    = m_params.agent.n_timesteps_go_to_pub;
      m_t_spent_at_pub = 0;
      m_is_at_pub      = false;
    }
  else // if (next_move == Move::Stay)
    {
      // Nothing to do here.
    }

  // Update contagion.
  if (m_state == State::Infected)
    {
      m_t_infection += 1;

      if (m_t_infection > m_params.agent.n_timesteps_recover)
        {
          m_state = State::Recovered;
        }
    }
}

std::pair<double, double> Agent::generate_direction_step() {
  // Since C++20.
  std::uniform_real_distribution<double> rand(0, 2 * std::numbers::pi);
  const double alpha = rand(m_engine);
  const double dx = m_params.agent.dr * std::cos(alpha);
  const double dy = m_params.agent.dr * std::sin(alpha);
  return std::make_pair(dx, dy);
}