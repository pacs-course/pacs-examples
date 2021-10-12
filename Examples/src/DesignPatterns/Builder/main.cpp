#include "graphentities.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
int
main()
{
  using namespace Graphs;
  std::cout << " Directed (0) or undirected (1) graph?" << std::endl;
  int ris;
  std::cin >> ris;
  std::unique_ptr<GraphBuilder> gb;

  if(ris == 0)
    gb = std::make_unique<DirectedGraphBuilder>();
  else
    gb = std::make_unique<UndirectedGraphBuilder>();

  std::ifstream file("graph.dat");
  // I just read edges for simplicity otherwise I need to parse the file
  basic_edge   be;
  unsigned int edgeid{0};
  while(!file.eof() && !file.fail())
    {
      file >> be[0] >> be[1];
      if(!file.fail())
        gb->buildEdges(be, edgeid++);
    }
  Graph &g = gb->getGraph();
  std::cout << " Nodes: \n";
  for(auto const &n : g.nodes)
    std::cout << " Node Id: " << n.getId() << "\n";
  std::cout << " Edges: \n";
  for(auto const &n : g.edges)
    std::cout << "Edge Id. " << n.getId() << ", (" << n.getEdgeNodes()[0]
              << ", " << n.getEdgeNodes()[1] << ")" << std::endl;
}
