#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include "graphentities.hpp"
int main()
{
  using namespace Graphs;
  std::cout<<" Directed (0) or undirected (1) graph?"<<std::endl;
  int ris;
  std::cin>>ris;
  std::unique_ptr<GraphBuilder> gb;
  if (ris==0)
    gb.reset(new DirectedGraphBuilder());
  else
    gb.reset(new UndirectedGraphBuilder());
  std::ifstream file("graph.dat");
  // I just read edges for simplicity otherwise I need to parse the file
  basic_edge be;
  unsigned int edgeid{0};
  while (!file.eof() && ! file.fail())
    {
      file>>be[0]>>be[1];
      if (! file.fail())
        gb->buildEdges(be,edgeid++);
    }
  Graph & g = gb->get_graph();
  std::cout<<" Nodes: \n";
  for (auto const & n:g.nodes) std::cout<<n.get_id()<<" "<<n.get_element()<<std::endl;
  std::cout<<" Edges: \n";
  for (auto const & n:g.edges) std::cout<<n.get_id()<<" "<<n.get_element()[0]<<" "<<n.get_element()[1]<<std::endl;
}
