#include "Utilities.hpp"
#include <limits>
#include <set>
#include <queue>
#include <numeric>
#include <vector>


void LinearAlgebra::AdjacencyGraph::info(std::ostream & out, bool verbose) const
{
  std::vector<double> xx;

  out<<"Graph has "<<M_degree.size()<<" Nodes"<<std::endl;
  // COmpute bandwidth of each row
  // We are assuming that Index is a POD type (which is resonable)
  auto bignumber=std::numeric_limits<Index>::max();
  std::vector<unsigned int> bandwidth(M_degree.size());
  unsigned int count(0);
  for (auto i : M_graph)
    {
      Index bleft(bignumber);
      Index bright(0);
      for (auto j : i)
        {
          bleft = std::min(bleft,j);
            bright= std::max(bright,j);
        }
        // Take into account a node with no connection
        // It is strange since it means that a row of the matrix
        // is zero, but I cannot exclude it. In this case the
        // bandwidth is zero for that row
      if (bleft==bignumber)
        bandwidth[count]=0;
      else
        bandwidth[count]=bright - bleft + 1u; // 1u is 1 as unsigned
      // adjourn row counter
      count++;
    }
    // output bandwidth info
  auto maxbi = std::max_element(bandwidth.begin(),bandwidth.end());
  auto minbi = std::min_element(bandwidth.begin(),bandwidth.end());
  out<<"Matrix      bandwidth: "<< *maxbi<<std::endl;
  out<<"Minimal row bandwidth: "<< *minbi<<std::endl;
  out<<"Average row bandwidth: "<<std::accumulate(bandwidth.begin(), bandwidth.end(), 0) / static_cast<double>(bandwidth.size())<<std::endl;
  std::vector<double> fillRatio;
  fillRatio.reserve(bandwidth.size());
  for (std::size_t i=0;i<bandwidth.size();++i)
    fillRatio.push_back(M_graph[i].size()/static_cast<double>(bandwidth[i]));
  auto maxfr = std::max_element(fillRatio.begin(),fillRatio.end());
  auto minfr = std::min_element(fillRatio.begin(),fillRatio.end());
  out<<"Maximal fill ratio   : "<< *maxfr<<std::endl;
  out<<"Minimal fill ratio   : "<< *minfr<<std::endl;
  out<<"Average fill ratio   : "<<std::accumulate(fillRatio.begin(), fillRatio.end(), 0.0) / static_cast<double>(fillRatio.size())<<std::endl;
  
  if (verbose)
    {
      out<<" Bandwidth of each row and fill ratio :"<<std::endl;
      for (std::size_t i=0;i<bandwidth.size();i++)
        out<<i<<": "<<bandwidth[i]<<"  "<<fillRatio[i]<<std::endl;
    }
}

std::vector<LinearAlgebra::AdjacencyGraph::Index> LinearAlgebra::applyRCMK(LinearAlgebra::AdjacencyGraph const & g, bool reverse)
{
  // The type to address indexes (typycally unsigned int)
  using Index=LinearAlgebra::AdjacencyGraph::Index;
  // Vector to store the reodered indexes.
  std::vector<Index> R;
  R.reserve(g.num_nodes());
  // Vector to mark visited nodes. Initialised to false
  std::vector<bool> visited(g.num_nodes(),false);
  //! Compare function to order priority queue by node degree with smallest on top
  auto cmp = [&g](Index i, Index j){return g.degree(i)>g.degree(j);};
  // Initial nodes, stored in a set I need the ordring from smallest
  // to grater, so I need to change the comparison!
  auto cmpr = [&g](Index i, Index j){return g.degree(i)<g.degree(j);};
  std::multiset<Index,decltype(cmpr)> s(cmpr);
  for (Index i=0;i<g.num_nodes();++i) s.insert(i);
  // Priority queue next level set (more efficient than a set)
  std::priority_queue<Index,std::vector<Index>,decltype(cmp)> adj(cmp);
  //! the type to address a vector. I use it as a counter
  std::size_t i(0);
  Index currentIndex;
  while (R.size() < g.num_nodes())
    {
      if (R.size()==i)
        {
          // I am at the beginning or the iteration or the graph is disconnected
          // Take out processed index from the set and get the first unprocessed one
          for (auto & j: s)
            if (!visited[j])
              {
                currentIndex = j;
                break;
              }
          visited[currentIndex]=true;
          R.emplace_back(currentIndex);
        }
      // Get next node and ajourn counter
      currentIndex= R[i++];
      // Look at all adjacent nodes not market yet
      auto adNodes=g.ad(currentIndex);
      for (auto j : adNodes)
        if (visited[j]==false)
          {
            adj.push(j);
            visited[j]=true;
          }
      while (!adj.empty())
        {
          // Add the nodes to R
          R.emplace_back(adj.top());
          // Delete from priority list
          adj.pop();
        }
    }
  // Reverse R (is reverse MKee!)
  if (reverse) std::reverse(R.begin(),R.end());
  //  for (auto j : R)std::cout<<j<<" ";
  //std::cout<<std::endl;
  return R;
}

