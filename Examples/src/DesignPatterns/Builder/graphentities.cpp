#include "graphentities.hpp"
namespace Graphs
{
void
DirectedGraphBuilder::buildNode(unsigned int id)
{
  G.nodes.emplace(id);
}
void
DirectedGraphBuilder::buildEdges(basic_edge e, unsigned int id)
{
  G.edges.emplace(e, id);
  // add also the nodes
  G.nodes.emplace(e[0]);
  G.nodes.emplace(e[1]);
}
void
UndirectedGraphBuilder::buildNode(unsigned int id)
{
  G.nodes.emplace(id);
}

void
UndirectedGraphBuilder::buildEdges(basic_edge e, unsigned int id)
{
  G.edges.emplace(e, id);
  // add the opposite edge
  std::swap(e[0], e[1]);
  G.edges.emplace(e, id);
  // add also the nodes
  G.nodes.emplace(e[0]);
  G.nodes.emplace(e[1]);
}
} // namespace Graphs
