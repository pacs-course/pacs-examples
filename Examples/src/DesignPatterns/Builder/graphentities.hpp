#ifndef HH_GRAPH_ENTITIES
#define HH_GRAPH_ENTITIES
#include <array>
#include <limits>
#include <set>
namespace Graphs
{
//! A simple example of classes storing a graph
//! Only to show the build pattern!

//! Indicates an unset identifier
/*!
  in c++17 we could use std::optional
 */
constexpr unsigned int nullID = std::numeric_limits<unsigned int>::max();

//! A basic edge is identified by the id of the two ends
using basic_edge = std::array<unsigned int, 2>;

class element
{
public:
  element(unsigned int i = nullID) : id{i} {}
  unsigned int
  getId() const
  {
    return id;
  }
  void
  setId(unsigned int r)
  {
    id = r;
  }
  virtual ~element() = default;

protected:
  unsigned int id;
};

class node : public element
{
public:
  node(unsigned int const &i = nullID) : element{i} {};
};

class edge : public element
{
public:
  edge() = default;
  // edge ends and id
  edge(basic_edge const &e, unsigned int i = nullID) : element{i}, M_edge{e} {}
  // Returns ids of edges ends
  basic_edge
  getEdgeNodes() const
  {
    return M_edge;
  };
  void
  setEdgeNodes(basic_edge const &a)
  {
    M_edge = a;
  };

private:
  basic_edge M_edge{nullID, nullID};
};
} // namespace Graphs

//! Comparison operators implemented by specialization of std::less
template <> struct std::less<Graphs::node>
{
  bool
  operator()(const Graphs::node &a, const Graphs::node &b) const
  {
    return a.getId() < b.getId();
  }
};

//! Comparison operation for edges
/*
  It relies on the lexicografic comparison implemented in basic_edge.
  Note that with this implementation edge (a,b) is different than edge (b,a)
  (directed graph)
 */
template <> struct std::less<Graphs::edge>
{
  bool
  operator()(const Graphs::edge &a, const Graphs::edge &b) const
  {
    return a.getEdgeNodes() < b.getEdgeNodes();
  }
};

namespace Graphs
{
//! A simple struct to hold a graph
/*!
  It can be converted to a class with the appropriate getters and setters.
  But for this example a simple struct is enough.
 */
struct Graph
{
  std::set<node> nodes;
  std::set<edge> edges;
};

//! Base class
class GraphBuilder
{
public:
  virtual void buildNode(unsigned int id) = 0;
  virtual void buildEdges(basic_edge e, unsigned int id) = 0;
  Graph &
  getGraph()
  {
    return G;
  }
  virtual ~GraphBuilder() = default;

protected:
  // Here I use an object since I have just one type of graphs
  // In general I can use a pointer if I have a polymorphic family of graphs
  Graph G;
};

//! To build undirected graph
class UndirectedGraphBuilder : public GraphBuilder
{
public:
  void buildNode(unsigned int) override;
  //! I give nodes id eand edge id
  void buildEdges(basic_edge, unsigned int) override;
};

//! To build directed graphs
class DirectedGraphBuilder : public GraphBuilder
{
public:
  void buildNode(unsigned int) override;
  void buildEdges(basic_edge, unsigned int) override;
};

} // namespace Graphs
#endif
