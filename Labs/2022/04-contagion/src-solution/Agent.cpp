#include "Agent.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <execution>
#include <limits>

// Since C++20.
#include <numbers>

Agent::Agent(const State &initial_state, const Parameters &params)
  : params(params)
  , engine(std::chrono::system_clock::now().time_since_epoch().count())
  , does_sd(false)
  , state(initial_state)
  , t_infection(0)
  , is_at_pub(false)
  , t_spent_at_pub(0)
{
  std::uniform_real_distribution<double> rand(0, 1);

  x = rand(engine) * params.domain.domain_size;
  y = rand(engine) * params.domain.domain_size;

  does_sd = (rand(engine) < params.contagion.frac_sd);

  std::uniform_int_distribution<unsigned int> randi(
    1, params.agent.n_timesteps_go_to_pub);
  t_go_to_pub = randi(engine);
}

void
Agent::move()
{
  if (t_go_to_pub.has_value())
    --(t_go_to_pub.value());

  if (is_at_pub)
    {
      ++t_spent_at_pub;
    }
  else
    {
      x_bak = x;
      y_bak = y;
    }

  // Determine next move.
  Move next_move;
  if (t_go_to_pub > 0)
    {
      if (does_sd)
        {
          next_move = Move::Stay;
        }
      else
        {
          next_move = Move::Walk;
        }
    }
  else if (is_at_pub)
    {
      if (t_spent_at_pub < params.agent.n_timesteps_at_pub)
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
      // Generate new random direction step.
      auto generate_direction_step = [this]() -> std::pair<double, double> {
        // Since C++20.
        std::uniform_real_distribution<double> rand(0, 2 * std::numbers::pi);

        const double alpha = rand(engine);

        const double dx = params.agent.dr * std::cos(alpha);
        const double dy = params.agent.dr * std::sin(alpha);

        return std::make_pair(dx, dy);
      };

      auto [dx, dy] = generate_direction_step();

      double x_new = x + dx;
      double y_new = y + dy;

      // If a wall is hit, try new random directions.
      auto wall_hit = [this](const double &x, const double &y) -> bool {
        return ((x < 0) || (x > params.domain.domain_size) || (y < 0) ||
                (y > params.domain.domain_size));
      };

      unsigned int n_tries      = 0;
      bool         still_trying = true;
      while (wall_hit(x_new, y_new) && still_trying)
        {
          std::tie(dx, dy) = generate_direction_step();

          x_new = x + dx;
          y_new = y + dy;

          if ((++n_tries) >= 1000)
            {
              // Don't move at all.
              x_new = x;
              y_new = y;

              still_trying = false;
            }
        }

      x = x_new;
      y = y_new;
    }
  else if (next_move == Move::Go_To_Pub)
    {
      std::uniform_real_distribution<double> rand_x(
        params.domain.pub_x - 0.5 * params.domain.pub_size,
        params.domain.pub_x + 0.5 * params.domain.pub_size);

      std::uniform_real_distribution<double> rand_y(
        params.domain.pub_y - 0.5 * params.domain.pub_size,
        params.domain.pub_y + 0.5 * params.domain.pub_size);

      x = rand_x(engine);
      y = rand_y(engine);

      t_go_to_pub = {};
      is_at_pub   = true;
    }
  else if (next_move == Move::Return_From_Pub)
    {
      x = x_bak;
      y = y_bak;

      t_go_to_pub    = params.agent.n_timesteps_go_to_pub;
      t_spent_at_pub = 0;
      is_at_pub      = false;
    }
  else // if (next_move == Move::Stay)
    {
      // Nothing to do here.
    }
}

void
Agent::update_contagion(std::vector<Agent> &agents)
{
  // Update contagion.
  if (state == State::Infected)
    {
      t_infection += 1;

      if (t_infection > params.agent.n_timesteps_recover)
        {
          state = State::Recovered;
        }
    }

  if (state == State::Infected)
    {
      std::for_each(std::execution::par_unseq,
                    agents.begin(),
                    agents.end(),
                    [this](Agent &other) {
                      const double x_dist = x - other.x;
                      const double y_dist = y - other.y;
                      const double r =
                        std::sqrt(x_dist * x_dist + y_dist * y_dist);

                      // If "other" is me.
                      if (r <= std::numeric_limits<double>::epsilon())
                        return;

                      const bool im_home = ((x == x_bak) && (y == y_bak));

                      const bool other_met =
                        (r <= params.contagion.r_infection);

                      const bool other_is_home =
                        ((other.x == other.x_bak) && (other.y == other.y_bak));

                      if (!im_home && other_met && !other.recovered() &&
                          !other_is_home)
                        {
                          other.state = State::Infected;
                        }

                      return;
                    });
    }
}
