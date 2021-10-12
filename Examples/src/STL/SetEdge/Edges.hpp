#ifndef HH__Edges_HPP
#define HH__Edges_HPP
#include <array>
//! A simple class storing the Ids of Points in an Edge
class Edge
{
public:
  //! Constructor taking the point Id
  Edge(const unsigned &a, const unsigned &b) : M_pointId{{a, b}} {}
  //! Extracts the id of the Ith point
  unsigned int
  operator[](unsigned int i) const
  {
    return M_pointId[i];
  }
  //! Extracts the id of the Ith point, non const version
  unsigned int &
  operator[](unsigned int i)
  {
    return M_pointId[i];
  }
  //! Returns both points ID, only const version
  std::array<unsigned, 2>
  getPointId() const
  {
    return M_pointId;
  }

private:
  //! Arrray storing the points
  std::array<unsigned, 2> M_pointId;
};

struct yesOrient
{
  bool operator()(Edge const &a, Edge const &b) const;
};

struct noOrient
{
  bool operator()(Edge const &a, Edge const &b) const;
};

class Triangle
{
public:
  //! Constructor taking the point Id
  Triangle(unsigned int a, unsigned int b, unsigned int c)
    : M_pointId{{a, b, c}}
  {}
  //! Estracts the id of the Ith point
  unsigned int
  operator[](unsigned int i) const
  {
    return M_pointId[i];
  }
  //! Estracts the id of the Ith point, non const version
  unsigned int &
  operator[](unsigned int i)
  {
    return M_pointId[i];
  }
  //! Returns both points ID, only const version
  std::array<unsigned, 3>
  getPointId() const
  {
    return M_pointId;
  }

  std::array<Edge, 3>
  edges() const
  {
    return {Edge{M_pointId[0], M_pointId[1]}, Edge{M_pointId[1], M_pointId[2]},
            Edge{M_pointId[2], M_pointId[0]}};
  }

private:
  //! Arrray storing the points
  std::array<unsigned, 3> M_pointId;
};

#endif
