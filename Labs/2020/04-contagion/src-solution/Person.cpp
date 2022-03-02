#include "Person.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <execution>
#include <limits>

Person::Person(const State &           initial_state,
               const PersonParameters &params)
  : params(params)
  , engine(
      std::chrono::system_clock::now().time_since_epoch().count())
  , rand(0, 1)
  , randi(1, params.n_timesteps_go_to_market)
  , does_sd(false)
  , state(initial_state)
  , t_infection(0)
  , is_at_market(false)
  , t_spent_at_market(0)
{
  x = rand(engine) * params.params_contagion->domain_size;
  y = rand(engine) * params.params_contagion->domain_size;

  if (rand(engine) < params.params_contagion->frac_sd)
    does_sd = true;

  t_go_to_market = randi(engine);
}

std::pair<double, double>
Person::generate_direction()
{
  double alpha = 2 * M_PI * rand(engine);

  double dx = params.dr * std::sin(alpha);
  double dy = params.dr * std::cos(alpha);

  return std::make_pair(dx, dy);
}

void
Person::move()
{
  // Move.
  t_go_to_market -= 1; // Update time to go to market.

  if (is_at_market)
    {
      t_spent_at_market += 1;
    }
  else
    {
      x_bak = x;
      y_bak = y;
    }

  Move next_move;
  if (t_go_to_market > 0)
    {
      if (!does_sd)
        {
          next_move = Move::Walk;
        }
      else
        {
          next_move = Move::Stay;
        }
    }
  else if (is_at_market)
    {
      if (t_spent_at_market < params.n_timesteps_at_market)
        {
          next_move = Move::Stay;
        }
      else
        {
          next_move = Move::Return_From_Market;
        }
    }
  else // if (t_go_to_market == 0)
    {
      next_move = Move::Go_To_Market;
    }

  if (next_move == Move::Walk)
    {
      double dx, dy;

      std::tie(dx, dy) = generate_direction();

      double x_new = x + dx;
      double y_new = y + dy;

      // If a wall is hit, try new random directions.
      bool wall_hit =
        ((x_new < 0) ||
         (x_new > params.params_contagion->domain_size) ||
         (y_new < 0) ||
         (y_new > params.params_contagion->domain_size));

      unsigned int n_tries      = 0;
      bool         still_trying = true;

      while (wall_hit && still_trying)
        {
          std::tie(dx, dy) = generate_direction();

          x_new = x + dx;
          y_new = y + dy;

          wall_hit =
            ((x_new < 0) ||
             (x_new > params.params_contagion->domain_size) ||
             (y_new < 0) ||
             (y_new > params.params_contagion->domain_size));

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
  else if (next_move == Move::Go_To_Market)
    {
      x = params.params_contagion->market_x +
          (rand(engine) - 0.5) * params.params_contagion->market_size;

      y = params.params_contagion->market_y +
          (rand(engine) - 0.5) * params.params_contagion->market_size;

      t_go_to_market = -1;
      is_at_market   = true;
    }
  else if (next_move == Move::Return_From_Market)
    {
      x = x_bak;
      y = y_bak;

      t_go_to_market    = params.n_timesteps_go_to_market;
      t_spent_at_market = 0;
      is_at_market      = false;
    }
  else // if (next_move == Move::Stay)
    {
      // Do nothing.
    }
}

void
Person::update_contagion(std::vector<Person> &people)
{
  // Update contagion.
  if (state == State::Infected)
    {
      t_infection += 1;

      if (t_infection > params.n_timesteps_recover)
        {
          state = State::Recovered;
        }
    }

  if (state == State::Infected)
    {
      std::for_each(
        std::execution::par_unseq,
        people.begin(),
        people.end(),
        [this](Person &other) {
          double x_dist = x - other.x;
          double y_dist = y - other.y;
          double r = std::sqrt(x_dist * x_dist + y_dist * y_dist);

          // If "other" is me.
          if (r <= std::numeric_limits<double>::epsilon())
            return;

          bool other_met =
            (r <= params.params_contagion->r_infection);

          bool other_is_home =
            ((other.x == other.x_bak) && (other.y == other.y_bak));

          if (other_met && !other.recovered() && !other_is_home)
            {
              other.state = State::Infected;
            }

          return;
        });
    }
}
