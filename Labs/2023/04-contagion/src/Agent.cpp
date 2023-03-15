#include "Agent.hpp"

#include <numbers>

Agent::Agent(size_t seed, const State &initial_state, const Parameters &params)
  : m_params(params)
  , m_engine(seed)
  , m_state(initial_state)
{
  std::uniform_real_distribution<double> rand(0, 1);
  m_x = rand(m_engine) * params.domain.domain_size;
  m_y = rand(m_engine) * params.domain.domain_size;
}

void
Agent::move()
{
  // try to move until we do not hit a wall or exceed maximum
  // tries and we stay still
  for(int i = 0; i < 1000; ++i) {
    const auto [dx, dy] = generate_direction_step();
    const auto xnew = m_x + dx;
    const auto ynew = m_y + dy;
    const bool inside = (xnew > 0) && (xnew <= m_params.domain.domain_size) &&
                        (ynew > 0) && (ynew <= m_params.domain.domain_size);
    if(inside) {
      m_x = xnew;
      m_y = ynew;
      break;
    }
  }
}

std::pair<double, double> Agent::generate_direction_step() {
  std::uniform_real_distribution<double> dist(0, 2.0*std::numbers::pi);
  const double alpha = dist(m_engine);
  const double dx = m_params.agent.dr * std::sin(alpha);
  const double dy = m_params.agent.dr * std::cos(alpha);
  return {dx, dy};
}
