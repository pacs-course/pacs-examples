#ifndef HH_GRAPH_ENTITIES
#define HH_GRAPH_ENTITIES
#include <limits>
#include <array>
#include <set>
namespace Graphs
{
  // A simple example of classes storing a graph
  // Only to show the build pattern!

  //! Indicated a non set identifier
  constexpr unsigned int nullID=std::numeric_limits<unsigned int>::max();

  using basic_edge=std::array<unsigned int,2>;
  
  class element
  {
  public:
    element(unsigned int i=nullID):id{i}{}
    unsigned int get_id()const {return id;}
    void set_id(unsigned int r){id=r;}
    virtual ~element()=default; 
  protected:
    unsigned int id;
  };
  
  class node: public element
  {
  public:
    node (unsigned int const & i=nullID):element{i}{};
    // Returns node id if node
    unsigned int get_element() const {return id;};
    void set_element(unsigned int const & a) {id=a;};
  };
  
  class edge: public element
  {
  public:
    // edge ends and id
    edge(basic_edge const & e={nullID,nullID},unsigned int i=nullID):
      element{i},M_edge{e}{}
    // Returns ids of edges ends
    basic_edge get_element() const {return M_edge;};
    void set_element(basic_edge const & a) {M_edge=a;};
  private:
    basic_edge M_edge;
  };
}
// COmparison operators
// in namespace std, so I have to take out Graphs
namespace std
{
template <>
struct less<Graphs::node>
{ 
  bool operator()(const Graphs::node & a, const Graphs::node & b)
  {
    return a.get_element()<b.get_element();  
  }
};

template <>
struct less<Graphs::edge>
{ 
  bool operator()( const Graphs::edge & a, const Graphs::edge & b)
  {
    return a.get_element()<b.get_element();  
  }
};
}

namespace Graphs
{
  struct Graph
  {
    std::set<node> nodes;
    std::set<edge> edges;
  };
  
  class GraphBuilder
  {
  public:
    virtual void buildNode(unsigned int id)=0;
    virtual void buildEdges(basic_edge e, unsigned int id)=0;
    Graph & get_graph(){return G;}
    virtual ~GraphBuilder()=default;
  protected:
    // Here I use an object since I have just one type of graphs
    // In general I can use a pointer if I have a polymorphic family of graphs
    Graph G;
  };
  
  class UndirectedGraphBuilder: public GraphBuilder
  {
  public:
    void buildNode(unsigned int) override;
    //! I give nodes id eand edge id
    void buildEdges(basic_edge, unsigned int) override;
  };
  
  class DirectedGraphBuilder: public GraphBuilder
  {
  public:
    void buildNode(unsigned int) override;
    void buildEdges(basic_edge, unsigned int) override;
  };

  
}// end Graph
#endif
