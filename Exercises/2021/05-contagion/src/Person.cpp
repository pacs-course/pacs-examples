#include "Person.hpp"

#include <chrono>

Person::Person(const std::string &filename, const State &initial_state)
  : domain_params(filename)
  , contagion_params(filename)
  , params(filename)
  , engine(std::chrono::system_clock::now().time_since_epoch().count())
  , rand(0, 1)
  , randi(1, params.n_timesteps_go_to_pub)
  , state(initial_state)
  , x(rand(engine) * domain_params.domain_size)
  , y(rand(engine) * domain_params.domain_size)
  , does_sd(false)
  , t_infection(0)
  , is_at_pub(false)
  , t_go_to_pub(randi(engine))
  , t_spent_at_pub(0)
{
  if (rand(engine) < contagion_params.frac_sd)
    {
      does_sd = true;
    }
}

void
Person::move()
{
  t_go_to_pub -= 1;

  if (is_at_pub)
    {
      t_spent_at_pub += 1;
    }
  else
    {
      x_old = x;
      y_old = y;
    }

  Move next_move;
  if (t_go_to_pub > 0)
    {
      if (does_sd)
        next_move = Move::Stay;
      else
        next_move = Move::Walk;
    }
  else if (is_at_pub)
    {
      if (t_spent_at_pub == params.n_timesteps_at_pub)
        next_move = Move::Return_From_Pub;
      else
        next_move = Move::Stay;
    }
  else // if (t_go_to_pub == 0)
    {
      next_move = Move::Go_To_Pub;
    }

  if (next_move == Move::Walk)
    {
      // Generate random step and update (x, y).
      const double alpha = 2 * M_PI * rand(engine);

      const double dx = params.dr * std::cos(alpha);
      const double dy = params.dr * std::sin(alpha);

      x += dx;
      y += dy;

      // Check that the new x and y are not outside the domain.
      if (x < 0)
        x = 0;
      else if (x > domain_params.domain_size)
        x = domain_params.domain_size;

      if (y < 0)
        y = 0;
      else if (y > domain_params.domain_size)
        y = domain_params.domain_size;
    }
  else if (next_move == Move::Go_To_Pub)
    {
      // Generate random position inside the pub domain.
      // [pub_x - pub_size / 2, pub_x + pub_size / 2].
      x = domain_params.pub_x + (rand(engine) - 0.5) * domain_params.pub_size;
      y = domain_params.pub_y + (rand(engine) - 0.5) * domain_params.pub_size;

      t_go_to_pub = -1;
      is_at_pub   = true;
    }
  else if (next_move == Move::Return_From_Pub)
    {
      // Restore old position.
      x = x_old;
      y = y_old;

      // Reset counters.
      t_go_to_pub    = params.n_timesteps_go_to_pub;
      t_spent_at_pub = 0;
      is_at_pub      = false;
    }
  else // if (next_move == Move::Stay)
    {
      // Do nothing.
    }
}

void
Person::update_infection(std::vector<Person> &people)
{
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
      // people closer than "r_infection" get infected
      for (size_t n = 0; n < people.size(); ++n)
        {
          // compute distance
          const double x_dist = x - people[n].x;
          const double y_dist = y - people[n].y;

          const double distance = std::sqrt(x_dist * x_dist + y_dist * y_dist);

          if (distance < 1e-12) // people[n] is me.
            continue;

          // If "other" is close and has not previously recovered.
          if (distance < contagion_parameters.r_infection &&
              people[n].state == State::Susceptible)
            {
              people[n].state = State::Infected;
            }
        }
    }
}
