#include "Contagion.hpp"

#include "gnuplot-iostream.hpp"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>

Contagion::Contagion(const ContagionParameters &params_)
  : params(params_)
  , rand(0, 1)
  , randi(1, params.n_timesteps_go_to_market)
{
  x.resize(params.n_people);
  y.resize(params.n_people);
  alpha.resize(params.n_people);
  dx.resize(params.n_people);
  dy.resize(params.n_people);

  x_bak.resize(params.n_people);
  y_bak.resize(params.n_people);

  does_sd.assign(params.n_people, false);

  is_susceptible.assign(params.n_people, true);
  is_infected.assign(params.n_people, false);
  is_recovered.assign(params.n_people, false);
  t_infected.assign(params.n_people, 0);

  t_go_to_market.resize(params.n_people);

  t_at_market.assign(params.n_people, 0);
  is_at_market.assign(params.n_people, false);

  for (unsigned int p = 0; p < params.n_people; ++p)
    {
      x[p]     = rand(engine);
      y[p]     = rand(engine);
      alpha[p] = 2 * M_PI * rand(engine);
      dx[p]    = params.dr * std::sin(alpha[p]);
      dy[p]    = params.dr * std::cos(alpha[p]);

      if (rand(engine) < params.sd_frac)
        does_sd[p] = true;

      t_go_to_market[p] = randi(engine);
    }

  size_t seed       = 1;
  is_infected[seed] = true;
  does_sd[seed]     = false;
}

void
Contagion::simulate()
{
  std::cout << "Running simulation..." << std::endl;

  std::vector<double>       day(params.n_timesteps, 0);
  std::vector<unsigned int> n_infected(params.n_timesteps, 0);
  std::vector<unsigned int> n_recovered(params.n_timesteps, 0);
  std::vector<unsigned int> n_susceptible(params.n_timesteps, 0);

  for (unsigned int step = 0; step < params.n_timesteps; ++step)
    {
      day[step] =
        static_cast<double>(step + 1) / params.n_timesteps_per_day;

      for (unsigned int p = 0; p < params.n_people; ++p)
        {
          // Move.
          t_go_to_market[p] -= 1; // Update time to go to market.

          if (is_at_market[p])
            {
              t_at_market[p] += 1;
            }
          else
            {
              x_bak[p] = x[p];
              y_bak[p] = y[p];
            }

          std::string next_move;
          if (t_go_to_market[p] > 0)
            {
              if (!does_sd[p])
                {
                  if (params.verbose)
                    {
                      std::cout << "Day " << day[step] << ": person "
                                << p << " walks." << std::endl;
                    }

                  next_move = "walk";
                }
              else
                {
                  if (params.verbose)
                    {
                      std::cout << "Day " << day[step] << ": person "
                                << p << " stays at home."
                                << std::endl;
                    }

                  next_move = "stay";
                }
            }
          else if (is_at_market[p])
            {
              if (t_at_market[p] < params.n_timesteps_at_market)
                {
                  if (params.verbose)
                    {
                      std::cout << "Day " << day[step] << ": person "
                                << p << " stays at market."
                                << std::endl;

                      next_move = "stay";
                    }
                  else
                    {
                      if (params.verbose)
                        std::cout
                          << "Day " << day[step] << ": person " << p
                          << " returns from the market." << std::endl;
                    }

                  next_move = "return_from_market";
                }
            }
          else // if (t_go_to_market[p] == 0)
            {
              if (params.verbose)
                {
                  std::cout << "Day " << day[step] << ": person " << p
                            << " goes to the market." << std::endl;
                }

              next_move = "go_to_market";
            }

          if (next_move == "walk")
            {
              double x_new = x[p] + dx[p];
              double y_new = y[p] + dy[p];

              // If a wall is hit, try a random new direction, until
              // no walls is hit.
              bool wall_hit = ((x_new > 1) || (x_new < 0) ||
                               (y_new > 1) || (y_new < 0));

              unsigned int n_tries      = 0;
              bool         still_trying = true;

              while (wall_hit && still_trying)
                {
                  // Choose random new direction and update position.
                  alpha[p] = 2 * M_PI * rand(engine);
                  dx[p]    = params.dr * std::sin(alpha[p]);
                  dy[p]    = params.dr * std::cos(alpha[p]);

                  x_new = x[p] + dx[p];
                  y_new = y[p] + dy[p];

                  wall_hit = ((x_new > 1) || (x_new < 0) ||
                              (y_new > 1) || (y_new < 0));

                  n_tries += 1;

                  if (n_tries >= 1000)
                    {
                      // Don't move at all.
                      x_new = x[p];
                      y_new = y[p];

                      still_trying = false;
                    }
                }

              x[p] = x_new;
              y[p] = y_new;
            }
          else if (next_move == "go_to_market")
            {
              x[p] =
                params.market_x + rand(engine) * params.market_size;

              y[p] =
                params.market_y + rand(engine) * params.market_size;

              t_go_to_market[p] = -1;
              is_at_market[p]   = true;
            }
          else if (next_move == "return_from_market")
            {
              x[p] = x_bak[p];
              y[p] = y_bak[p];

              t_go_to_market[p] = params.n_timesteps_go_to_market;
              t_at_market[p]    = 0;
              is_at_market[p]   = false;
            }
          else // if (next_move == "stay")
            {
              // Do nothing.
            }

          // Update contagion.
          if (is_infected[p])
            {
              t_infected[p] += 1;

              if (t_infected[p] > params.n_timesteps_recover)
                {
                  is_infected[p]  = false;
                  is_recovered[p] = true;
                }
            }


          if (is_infected[p])
            {
              for (size_t other = 0; other < params.n_people; ++other)
                {
                  if (other != p)
                    {
                      bool met_other = false;

                      double x_dist = x[p] - x[other];
                      double y_dist = y[p] - y[other];
                      double r =
                        std::sqrt(x_dist * x_dist + y_dist * y_dist);

                      if (r <= params.r_infection)
                        {
                          met_other = true;
                        }

                      bool is_home_other =
                        ((x[other] == x_bak[other]) &&
                         (y[other] == y_bak[other]));

                      if (met_other && !is_recovered[other] &&
                          !is_home_other)
                        {
                          is_infected[other] = true;
                        }
                    }
                }
            }
        }

      // Update people count.
      for (unsigned int p = 0; p < params.n_people; ++p)
        {
          if (is_infected[p])
            ++n_infected[step];
          else if (is_recovered[p])
            ++n_recovered[step];
          else
            ++n_susceptible[step];
        }
    }

  // Export output.
  std::ofstream file("output.csv");
  file << "day, n_infected, n_recovered, n_susceptible" << std::endl;

  for (unsigned int step = 0; step < params.n_timesteps; ++step)
    {
      file << day[step] << ", " << n_infected[step] << ", "
           << n_recovered[step] << ", " << n_susceptible[step]
           << std::endl;
    }
  file.close();

  Gnuplot gp;
  gp << "set xlabel 'Day'; set ylabel 'No. of people'; plot"
     << gp.file1d(std::tie(day, n_infected))
     << "with line linewidth 2 title 'Infected',"
     << gp.file1d(std::tie(day, n_recovered))
     << "with line linewidth 2 title 'Recovered',"
     << gp.file1d(std::tie(day, n_susceptible))
     << "with line linewidth 2 title 'Susceptible'" << std::endl;
}
