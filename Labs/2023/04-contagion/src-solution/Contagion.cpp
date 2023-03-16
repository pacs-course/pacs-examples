#include "Contagion.hpp"

#include <algorithm>
#include <cmath>
#include <execution>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>
#include <chrono>
#include <iomanip>

#include <boost/preprocessor/stringize.hpp>

#ifndef L1_CACHE_SIZE
#define L1_CACHE_SIZE 0x0fffffffffffffffui64
#endif

#pragma message("Definig cache size to be = " BOOST_PP_STRINGIZE(L1_CACHE_SIZE))
constexpr auto agents_in_l1_cache = L1_CACHE_SIZE / sizeof(Agent);

Contagion::Contagion(const std::string &filename)
  : m_params(filename)
  , m_time(m_params.contagion.n_timesteps + 1)
  , m_n_susceptible(m_time.size())
  , m_n_infected(m_time.size())
  , m_n_recovered(m_time.size())
{
  m_agents.reserve(m_params.contagion.n_agents);

  std::default_random_engine engine(m_params.seed);
  for (size_t p = 0; p < m_agents.capacity() - m_params.contagion.n_init_infected;
       ++p)
    {
      m_agents.emplace_back(engine(), State::Susceptible, m_params);
    }

  for (size_t p = 0; p < m_params.contagion.n_init_infected; ++p)
    m_agents.emplace_back(engine(), State::Infected, m_params);
}

void
Contagion::run()
{
  simulate();
  output_results();
}

void
Contagion::simulate()
{
  {
  using namespace std::chrono;
  const auto t0 = high_resolution_clock::now();
  for (unsigned int step = 0; step <= m_params.contagion.n_timesteps; ++step)
    {
      //std::cout << "Timestep " << step << " / " << m_params.contagion.n_timesteps
      //          << "\r";

      m_time[step] =
        static_cast<double>(step) / m_params.contagion.n_timesteps_per_day;

      if (step >= 1)
        {
          // Here where are making an approximation w.r.t. the Matlab code
          // in particular we first simulate all the agents movements and than
          // we update the contagion.
          // This allows us to parallelize the code and showcase cache alignment
          // however the results will be different w.r.t. the original model
          std::for_each(std::execution::par_unseq,
                        m_agents.begin(),
                        m_agents.end(),
                        [this](Agent &p) {p.move();}
                      );

          // here we have a double nested for loop two check the distance between any
          // two agents. If certain conditions are met they become sick.
          // Here we can exploit the simmetry of the problem to limit the calculations
          // Moreorver we add a third for loop to hit the agents always in the same
          // memory zone, so to maximize the cache hits
          // In a real world scenario, instead of using a for loop and checking every 
          // pair it would be better to use a more sophisticated data structure like
          // * k-d tree https://en.wikipedia.org/wiki/K-d_tree
          // * Quadtree/Octree https://en.wikipedia.org/wiki/Quadtree
          // * Uniform grid searching method (if you divide the space in a uniform grid of 
          //    side greater than the contagion radius you can search contacts only in 
          //    contiguous cells)
          for(size_t k = 0; k < m_agents.size(); k += agents_in_l1_cache) {
            for(size_t i = 0; i < m_agents.size(); ++i) {
              for(size_t j = k; j < std::min(m_agents.size(), k + agents_in_l1_cache); ++j) {
                if((m_agents[i].susceptible() && m_agents[j].infected()) ||
                   (m_agents[j].susceptible() && m_agents[i].infected())) {
                  if(!(m_agents[i].is_home() || m_agents[j].is_home())) {
                    const auto dx = m_agents[i].x() - m_agents[j].x(); 
                    const auto dy = m_agents[i].y() - m_agents[j].y();
                    const auto r2 = dx * dx + dy * dy;
                    // we can avoid compute the square root
                    if(r2 <= m_params.contagion.r_infection_2) {
                      // one of them is already infected, we do not know which is
                      // to avoid checking we set both infected
                      m_agents[i].set_state(State::Infected);
                      m_agents[j].set_state(State::Infected);
                    }
                  } 
                }
              }
            }
          }
        }

      // Update agents count.
      m_n_susceptible[step] =
        std::count_if(std::execution::par_unseq,
                      m_agents.begin(),
                      m_agents.end(),
                      [](const Agent &p) { return p.susceptible(); });

      m_n_infected[step] =
        std::count_if(std::execution::par_unseq,
                      m_agents.begin(),
                      m_agents.end(),
                      [](const Agent &p) { return p.infected(); });

      m_n_recovered[step] =
        std::count_if(std::execution::par_unseq,
                      m_agents.begin(),
                      m_agents.end(),
                      [](const Agent &p) { return p.recovered(); });

      if(m_params.dump && ((step % m_params.dump) == 0)) {
        std::ostringstream oss;
        oss << "snapshot." << std::setfill('0') << std::setw(5) << step << ".csv";
        std::ofstream file(oss.str(), std::ofstream::out);
        file << "x,y,state\n"; 
        for(const auto &a : m_agents) {
          file << a.x() << "," << a.y() << "," << static_cast<int>(a.state()) << "\n";
        }
        file.close();
      }
    }
  const auto t1 = high_resolution_clock::now();
  std::cout << "Elapsed: " << duration_cast<milliseconds>(t1 - t0).count() << " [ms]" <<  std::endl;
  }

  // Find peak.
  const auto max_m_n_infected =
    std::max_element(m_n_infected.begin(), m_n_infected.end());

  const auto timestep_peak = std::distance(m_n_infected.begin(), max_m_n_infected);

  std::cout << "Peak of " << *max_m_n_infected << " infected ("
            << static_cast<double>(*max_m_n_infected) /
                 m_params.contagion.n_agents * 100
            << "%) reached after "
            << std::ceil(static_cast<double>(timestep_peak) /
                         m_params.contagion.n_timesteps_per_day)
            << " days." << std::endl;
}

void
Contagion::output_results() const
{
  // Output results to CSV file.
  std::ofstream file("output.csv", std::ofstream::out);
  file << "time,n_susceptible,n_infected,n_recovered\n";

  for (unsigned int step = 0; step <= m_params.contagion.n_timesteps; ++step)
    {
      file << m_time[step] << "," << m_n_susceptible[step] << ","
           << m_n_infected[step] << "," << m_n_recovered[step] << "\n";
    }
  file.close();
}
