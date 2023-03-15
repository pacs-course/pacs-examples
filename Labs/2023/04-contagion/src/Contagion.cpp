#include "Contagion.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <execution>

Contagion::Contagion(const std::string &filename)
  : m_params(filename)
  , m_time(m_params.contagion.n_timesteps + 1)
  , m_n_infected(m_time.size())
  , m_n_susceptible(m_time.size())
  , m_n_recovered(m_time.size())
{
  m_agents.reserve(m_params.contagion.n_agents);
  std::default_random_engine engine(m_params.seed);
  for(unsigned i = 0; i < m_params.contagion.n_init_infected; ++i)
    m_agents.emplace_back(engine(), State::Infected, m_params);
  for(unsigned i = 0; i < m_params.contagion.n_agents - m_params.contagion.n_init_infected; ++i)
    m_agents.emplace_back(engine(), State::Susceptible, m_params);
}

void
Contagion::run() {
  for (unsigned int step = 0; step <= m_params.contagion.n_timesteps; ++step) {
    m_time[step] = static_cast<double>(step) / m_params.contagion.n_timesteps_per_day;

    if (step >= 1) {
      for(auto &a : m_agents)
        a.move();
      for(size_t i = 0; i < m_agents.size(); i++) {
        for(size_t j = 0; j < m_agents.size(); ++j) {
          if((m_agents[i].susceptible() && m_agents[j].infected()) || 
             (m_agents[j].susceptible() && m_agents[i].infected()) ){
            const auto dx = m_agents[i].x() - m_agents[j].x();
            const auto dy = m_agents[i].y() - m_agents[j].y();
            const auto d2 = dx * dx + dy * dy;
            if(d2 <= m_params.contagion.r_infection_2) {
              m_agents[i].set_state(State::Infected);
              m_agents[j].set_state(State::Infected);
            }
          }
        }
      }
    }
    
    m_n_susceptible[step] = std::count_if(std::execution::par_unseq, m_agents.begin(), m_agents.end(), [&](auto &a){return a.susceptible();});
    m_n_infected[step] = std::count_if(std::execution::par_unseq, m_agents.begin(), m_agents.end(), [&](auto &a){return a.infected();});
    m_n_recovered[step] = std::count_if(std::execution::par_unseq, m_agents.begin(), m_agents.end(), [&](auto &a){return a.recovered();});

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

  output_results();
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
