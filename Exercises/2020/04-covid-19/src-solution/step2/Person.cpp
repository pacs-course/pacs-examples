#include "Person.hpp"

#include <chrono>
#include <limits>

Person::Person(const Status &initial_status)
  : engine(
      std::chrono::system_clock::now().time_since_epoch().count())
  , rand(0, 1)
  , randi(1, params.n_timesteps_go_to_market)
  , does_sd(false)
  , is_susceptible(initial_status == Status::Susceptible)
  , is_infected(initial_status == Status::Infected)
  , is_recovered(initial_status == Status::Recovered)
  , t_infected(0)
  , t_at_market(0)
  , is_at_market(false)
{
  x = rand(engine);
  y = rand(engine);

  alpha = 2 * M_PI * rand(engine);
  dx    = params.dr * std::sin(alpha);
  dy    = params.dr * std::cos(alpha);

  if (rand(engine) < params.frac_sd)
    does_sd = true;

  t_go_to_market = randi(engine);
}

void
Person::move()
{
  // Move.
  t_go_to_market -= 1; // Update time to go to market.

  if (is_at_market)
    {
      t_at_market += 1;
    }
  else
    {
      x_bak = x;
      y_bak = y;
    }

  std::string next_move;
  if (t_go_to_market > 0)
    {
      if (!does_sd)
        {
          next_move = "walk";
        }
      else
        {
          next_move = "stay";
        }
    }
  else if (is_at_market)
    {
      if (t_at_market < params.n_timesteps_at_market)
        {
          next_move = "stay";
        }
      else
        {
          next_move = "return_from_market";
        }
    }
  else // if (t_go_to_market == 0)
    {
      next_move = "go_to_market";
    }

  if (next_move == "walk")
    {
      double x_new = x + dx;
      double y_new = y + dy;

      // If a wall is hit, try a random new direction, until
      // no walls is hit.
      bool wall_hit =
        ((x_new > 1) || (x_new < 0) || (y_new > 1) || (y_new < 0));

      unsigned int n_tries      = 0;
      bool         still_trying = true;

      while (wall_hit && still_trying)
        {
          // Choose random new direction and update position.
          alpha = 2 * M_PI * rand(engine);
          dx    = params.dr * std::sin(alpha);
          dy    = params.dr * std::cos(alpha);

          x_new = x + dx;
          y_new = y + dy;

          wall_hit = ((x_new > 1) || (x_new < 0) || (y_new > 1) ||
                      (y_new < 0));

          n_tries += 1;

          if (n_tries >= 1000)
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
  else if (next_move == "go_to_market")
    {
      x = params.market_x + rand(engine) * params.market_size;

      y = params.market_y + rand(engine) * params.market_size;

      t_go_to_market = -1;
      is_at_market   = true;
    }
  else if (next_move == "return_from_market")
    {
      x = x_bak;
      y = y_bak;

      t_go_to_market = params.n_timesteps_go_to_market;
      t_at_market    = 0;
      is_at_market   = false;
    }
  else // if (next_move == "stay")
    {
      // Do nothing.
    }
}

void
Person::update_contagion(std::vector<Person> &people)
{
  // Update contagion.
  if (is_infected)
    {
      t_infected += 1;

      if (t_infected > params.n_timesteps_recover)
        {
          is_infected  = false;
          is_recovered = true;
        }
    }

  if (is_infected)
    {
      for (auto &other : people)
        {
          double x_dist = x - other.x;
          double y_dist = y - other.y;
          double r = std::sqrt(x_dist * x_dist + y_dist * y_dist);

          // If "other" is me.
          if (r <= std::numeric_limits<double>::epsilon())
            continue;

          bool other_met = (r <= params.r_infection);

          bool other_is_home =
            ((other.x == other.x_bak) && (other.y == other.y_bak));

          if (other_met && !other.is_recovered && !other_is_home)
            {
              other.is_infected = true;
            }
        }
    }
}
